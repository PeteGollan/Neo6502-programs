;---------------------------------------------------------------------
; read.s 
; int __fastcall__ read (int fd, void* buf, unsigned count);
;
; Adapted from the kim1 version by Pete Wyspianski (aka Gollan), July 2024
;
; NOTES:
;   * CC65 fgets() only asks for one character at a time.
;   * The O_RDONLY (read-only) and O_WRONLY (write-only) flags are NOT
;     exclusive. They just determine if a stream can be read or written.
;     Both flags can be set at the same time (+ mode).
;   * Read and write restrictions are software enforced in
;     the _read() and _write() functions, not the Neo6502 API.
;--------------------------------------------------------------------

        .include   "fcntl.inc"
        .include   "errno.inc"
        .include   "neo6502.asm.inc"

        .import         popax, popptr1
        .import         flags_tab
        .importzp       ptr1, ptr2, ptr3, tmp1

        .export         _read

        .segment "CODE"

.proc _read
        sta     ptr3
        stx     ptr3+1           ; Count in ptr3
        inx
        stx     ptr2+1           ; Increment and store in ptr2
        tax
        inx
        stx     ptr2
        jsr     popptr1          ; Buffer address in ptr1
        jsr     popax            ; fd in A
        sta     tmp1             ; fd in tmp1
        cmp     #STDIN_FILENO    ; Is this for STDIN?   
        bne     fbegin           ; Taken if NOT

; STDIN, console input routine
begin:  dec     ptr2
        bne     getch
        dec     ptr2+1
        beq     done             ; If buffer full, return

getch:  jsr     ReadCharacter    ; Get character using Monitor ROM call
        jsr     WriteCharacter   ; Echo character to screen (preserves registers)
        cmp     #$0D             ; Check for '\r'
        bne     putch            ; ...if CR character
        lda     #$0A             ; Replace with '\n'

putch:  ldy     #$00             ; Put char into return buffer
        sta     (ptr1),y
        inc     ptr1             ; Increment pointer
        bne     begin
        inc     ptr1+1
        bne     begin

done:   lda     ptr3
        ldx     ptr3+1
        rts                      ; Return count

; FILE input routine
; fd in A-reg and tmp1
; count in ptr3
; buffer address in ptr1

fbegin:     sta API_PARAM0      ; fd
; check if this stream has RDONLY set:
            tay                 ; fd
            lda flags_tab,y     ; Get file open flags for this fd
            and #O_RDONLY       ; can this stream be read?
            beq ferr            ; taken if NOT (not a readable stream)
; regular processing:
            lda ptr1            ; buffer pointer
            sta API_PARAM1
            lda ptr1+1
            sta API_PARAM2
            lda ptr3            ; size
            sta API_PARAM3
            lda ptr3+1
            sta API_PARAM4
            lda #API_FN_FILE_READ
            sta API_FUNCTION
            lda #API_GROUP_FILEIO
            sta API_COMMAND     ; Make it so.
            lda API_ERROR       ; How did it go?
            bne feof            ; Taken if error (assume EOF)
fdone:      lda API_PARAM3      ; bytes read
            ldx API_PARAM4
            rts

; Assume any API error is EOF:
 feof:      lda #$00    ; EOF return
            ldx #$00
            rts  

; Attempt to read a write-only stream.
 ferr:  lda #EACCES         ; Access denied
         jsr ___seterrno
         lda #$ff    ; error return
         ldx #$ff
         rts   
.endproc
