;------------------------------------------------------------------------
; open.s - int open(const char* name, int flags, ...); 
;
; By Pete Wyspianski (aka Gollan), July 2024
;
; NOTES:
;  * The Neo6502 API maximum filename length is 255 characters
;  * Drivers (_read and _write) enforce RDONLY and WRONLY, NOT the API
; 
; How _open() interacts with CC65 fopen() flags and the Neo6502 File API:
;+---------------------------+-------------------------------------------+
;|      Mode - Flags         |        Neo6502 File Open Mode             |
;+---------------------------+-------------------------------------------+ 
;| w - WRONLY CREAT TRUNC    | API mode 3 (RW OVERWRITE)                 |
;|                           | Existing file is overwritten
;+---------------------------+-------------------------------------------+ 
;| r - RDONLY                | API mode 2 (RW)                           |
;|                           | File must exist                           |
;+---------------------------+-------------------------------------------+ 
;| a - WRONLY CREAT APPEND   | API mode 2 (RW) followed by seek() to EOF.|
;|                           | If mode 2 fails(*), API mode 3, no seek().|
;|                           | File is either created or appended        | 
;+---------------------------+-------------------------------------------+
;(*) Mode 2 fails when the file doesn't already exist. In that case, we use
;    mode 3 to create a new file that will automatically be set for append.

    .export    _open
    .export     neo6502_fd, mask_tab, flags_tab
    .export     _makebstr, _freebstr
    .export     lptr1, lptr2
    .import     _malloc, _free, ___seterrno

    .import    addysp,popax

    .importzp  sp,ptr1,ptr2,tmp1,tmp2,tmp3

    .include   "errno.inc"
    .include   "fcntl.inc"
    .include   "neo6502.asm.inc"

.proc _open
; 
; Throw away any additional parameters passed through the ellipsis
            dey                 ; We expect 4 bytes of params (name ptr and int flags)
            dey
            dey
            dey
            beq parmok          ; Branch if parameter count ok
            jsr addysp          ; Fix stack, throw away unused parameters

parmok:     jsr popax           ; Get file open params
            sta tmp3            ; Save file open params (only use lsb)
            jsr popax           ; Get filename pointer
; Copy null-terminated string to length-prefixed string in a new memory block.
; AX has open file name ptr.
            jsr _makebstr
            cpy #0              ; good return?
            beq mbs_ok          ; taken if so
;_makebstr error exit. Errors are already handled.
            lda #$ff            ; error return
            ldx #$ff
            rts
            
mbs_ok:     sta API_PARAM1      ; save file name pointer
            stx API_PARAM2

; find a free fd:
            ldy #$00
            lda neo6502_fd
fd1:        bpl found_fd
            iny
            cpy  #$08    ; Are we out of fds?
            bne fd2      ; Take if not
; Out of fds, error exit
            lda #EMFILE     ; Too many open files     
            jsr ___seterrno
            jmp  errm     ; Exit if so
fd2:        asl          ; shift next bit into b7
            bra  fd1 
; Set the corresponding bit to show it is taken:
found_fd:   lda  mask_tab,y  ; get mask #y-reg (0-7)
            ora  neo6502_fd  ; set that bit
            sta  neo6502_fd
            tya              ; free fd -> A-reg
            clc
            adc #$03         ; Offset fd by 3 (range is 3-10)
            sta tmp1         ; Save fd

            tay              ; use offset fd as index into table
            lda tmp3         ; get file open params
            sta flags_tab,y  ; save for use in _read() and _write()

; flags are in tmp3, length-prefixed filename is already in PARAM1,2
; open the file with the Neo6502 API
            lda #API_FN_FILE_OPEN
            sta API_FUNCTION
            lda tmp1            ; fd
            sta API_PARAM0      ; fd param 0
            lda tmp3            ; file open params
            tay                 ; save file open params in y-reg
            and #O_RDONLY       ; 'r'? (A will = O_RDONLY if bit was set)
            beq chkmode2        ; Taken if NOT 'r'
;-------------------------------------------------------------
; r or r+
;-------------------------------------------------------------
mode_r:     tya                 ; file open params in A
            lda #API_FILE_OPEN_MODE_READ_WRITE    ; mode 2 
            bra set_open_mode
;-------------------------------------------------------------
; w or a - make sure it is w or a
;-------------------------------------------------------------
chkmode2:   tya                 ; file open params in A
            and #O_WRONLY       ; WRONLY is either 'w' or 'a'
            beq err_badfop      ; Taken if NOT 'w' or 'a'
; Is it 'w' or 'a'?
            tya                 ; file open params in A
            and #O_APPEND       ; APPEND is only 'a' or 'a+'
            beq mode_w          ; Taken if NOT (w or w+)
;-------------------------------------------------------------
; a or a+
;-------------------------------------------------------------
            LDA #API_FILE_OPEN_MODE_READ_WRITE  ; mode 2
            bra set_open_mode

; bad file open params (can only happen if called directly from user code)
err_badfop: lda #EINVAL         ; invalid argument
            jsr ___seterrno
            bra errfd

;-------------------------------------------------------------
; w or a are handled the same initially. Append has follow-up after the open.
;-------------------------------------------------------------
mode_w:     lda #API_FILE_OPEN_MODE_RW_OVERWRITE    ; file open mode 3
            bra set_open_mode

set_open_mode:
            sta API_PARAM3      ; The open mode (0-3)
            lda #API_GROUP_FILEIO
            sta API_COMMAND     ; Make it so.
            lda API_ERROR       ; How did it go?
            beq open_ok         ; zero means success.

; If we are doing append, try another open mode if the first causes an error:
            tya                 ; file open params in A
            and #O_APPEND       ; 
            beq err_api         ; Taken if NOT doing append (a real error)
; Append mode, but are we sill on the first try:
            lda API_PARAM3      ; which open mode was it?
            cmp #API_FILE_OPEN_MODE_RW_OVERWRITE  ; second mode to try
            beq err_api         ; taken if we already tried the second open mode
; Append mode, first try failed. Try the second mode:
            lda #API_FILE_OPEN_MODE_RW_OVERWRITE
            bra set_open_mode   ; try again

; open only returns '1' if there was an error, so return the most likely error.
err_api:    tya                 ; file open params in A
            and #O_RDONLY       ; if read, file must exist
            beq err_api_1        ; taken if not RDONLY
            lda #ENOENT         ; no such file
            bra err_api_2

err_api_1:  lda #EIO            ; generic IO error
err_api_2:  jsr ___seterrno
            bra errfd

; File is open. Check for additional handling for 'a' and 'a+' modes:
open_ok:    tya                 ; file open params in A
            and #O_APPEND       ; 
            beq exit_ok         ; Taken if NOT doing append
            lda API_PARAM3      ; get the open mode that was used
            cmp #API_FILE_OPEN_MODE_READ_WRITE ; is this the first try?
            bne exit_ok         ; Taken if NOT (second mode doesn't do seek())

; 'a' and 'a+' modes require a seek() to EOF after open (Neo6502 doesn't have append mode open):
; start with a call to File Size:
            lda #API_FN_FILE_SIZE
            sta API_FUNCTION  
; fd is already in API_PARAM0 from the previous call
            lda #API_GROUP_FILEIO
            sta API_COMMAND     ; Make it so.
            lda API_ERROR       ; How did it go?
            bne errfd           ; Taken if error
; fd is already in API_PARAM0 from the previous call
; The FILE SIZE is in API_PARAM1-4
; A File Seek with FILE SIZE as the offset will move the position to EOF
;
            lda #API_FN_FILE_SEEK 
            sta API_FUNCTION
            lda #API_GROUP_FILEIO
            sta API_COMMAND     ; Make it so.
            lda API_ERROR       ; How did it go?
            bne errfd           ; Taken if error

; File is open, return filename buffer and exit:
exit_ok:    lda ptr2    ; memory block, pointer in AX
            ldx ptr2+1
            jsr _freebstr
            lda tmp1    ; fd in A
            ldx #$00
            rts

; Error exit after fd has been found
errfd:      lda neo6502_fd
            ldy tmp1        ; fd in Y
            dey             ; subtract the 3 offset from Y
            dey
            dey
            lda mask_tab,y  ; get the mask for this fd
            eor #$ff        ; complement the mask
            and neo6502_fd  ; clear the target bit (free that fd)
            sta neo6502_fd
; Error exit after successful malloc
errm:     lda ptr2    ; memory block, pointer in AX
          ldx ptr2+1
          jsr _freebstr
; Error exit before malloc
err:      lda #$ff    ; error return
          ldx #$ff
          rts
.endproc


; freebstr(char *bstr)
;
; Frees a string buffer allocated by makebstr()
; Currently only calls _free(), but provide a hook for future additional processing.
; Inputs: AX -pointer to b-string

.proc _freebstr
    jsr _free
    rts
.endproc

;--------------------------------------------------------------------------------------
; char *bstr makebstr(const char *cstr)
;
; Copies a C-string (null-terminated string) to a B-string (length prefixed string)
; Calls _malloc() and uses ptr1 and ptr2
;
; Inputs: AX -pointer to c-string
; Outputs: AX - pointer to memory block with b-string
;          y-reg result code:  0 - success, 1 - malloc failed, 2- cstr is >255 bytes
; If the makebstr fails, any memory allocated is freed.
;--------------------------------------------------------------------------------------

.proc _makebstr
; ptr1 - source, null-terminated string
; ptr2 - dest, length prefixed string
; y-reg index is one ahead for dest as we're skipping the length byte
; This accomodates the two different styles of error handling in CC65: Some callers expect
; seterrno to already be called and we return -1 in AX. Others (the ones that call _sys* functions) expect
; a system specific error code in A-reg, zero in X-reg and they will call seterrno AFTER running the
; returned error code through mappederrno. For Neo6502, mappederrno does nothing becase we only use
; CC65 error codes.

            pha                 ; save params
            phx
            lda #$ff            ; fastcall, so params (size) in AX
            ldx #$00
            jsr _malloc         ; Returns buffer pointer in AX
            cpx #$00            ; check for NULL
            bne malloc_ok
            cmp #$00            
            bne malloc_ok
; ERROR: malloc failed, fix stack and exit:
            plx
            pla
            lda #ENOMEM         ; malloc failed, we're done.
            pha
            jsr ___seterrno
            pla                 ; ensure the error code is in A-reg...
            ldx #$0             ; and X-reg is zero
            ldy #$1             ; malloc failed return
            rts             

malloc_ok:  sta ptr2            ; Save dest string buffer
            stx ptr2+1          ; ptr2 points at the future length byte
            plx                 ; get source string pointer
            pla
            sta ptr1            ; Save filename ptr
            stx ptr1+1
            ldy #$00
copy_next:  lda (ptr1),y
            beq copy_done    ; found NULL terminator
            iny
            bne copy_cont    ; filename < 255 chars, keep going           
; ERROR: source name is > 255 chars, error exit:
            lda ptr2        ; get the malloc pointer
            ldx ptr2+1
            jsr _free       ; return it
            lda #EINVAL     ; invalid argumet is the best fit       
            pha
            jsr ___seterrno
            pla             ; ensure the error code is in A-reg...
            ldx #$0         ; and X-reg is zero
            ldy #$2         ; cstr > 255 bytes error
            rts     

copy_cont:  sta (ptr2),y
            bra copy_next
; y-reg has filename length:
copy_done:  tya
            sta (ptr2)   ; points at filename length byte
            lda ptr2     ; get the b-string pointer
            ldx ptr2+1
            ldy #$0      ; good return
            rts
.endproc
; ------------------------------------------------------------------------
; Pre-initialized data (will also be used by _close()

neo6502_fd:    .byte    $00     ; bit mask for open and free file channel numbers (8 values)

; this simplifies the code for both _open and _close:
mask_tab:   .byte $80   ; $00 -> fd $03
            .byte $40   ; $01 -> fd $04
            .byte $20   ; $02 -> fd $05
            .byte $10   ; $03 -> fd $06
            .byte $08   ; $04 -> fd $07
            .byte $04   ; $05 -> fd $08
            .byte $02   ; $06 -> fd $09
            .byte $01   ; $07 -> fd $0a

; Neo6502 API doesn't quite line up with ANSI C file open modes.
; To enforce read-only write-only, we have to do it in _read() and _write().
; CC65 doesn't send the file open mode flags down to _read() and _write(),
; so we will store them here, indexed by our own fd:
flags_tab:  
            .byte 00    ; NOT USED - saves some math in _read() and _write().
            .byte 00    ; NOT USED
            .byte 00    ; NOT USED
            .byte 00    ; fd $03
            .byte 00    ; fd $04
            .byte 00    ; fd $05
            .byte 00    ; fd $06
            .byte 00    ; fd $07
            .byte 00    ; fd $08
            .byte 00    ; fd $09
            .byte 00    ; fd $0a

; Local string pointers
lptr1:      .word 00
lptr2:      .word 00