;---------------------------------------------------------------------
; write.s 
; int __fastcall__ write (int fd, const void* buf, int count);
;
; Adapted from kim1 version by Pete Wyspianski (aka Gollan), July 2024
;
; NOTES:
;   * In testing, CC65 fgets() only asks for one character
;     at a time.
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

        .export         _write

        .segment "CODE"

.proc  _write

            sta     ptr3
            stx     ptr3+1          ; Count in ptr3
            inx
            stx     ptr2+1          ; Increment and store in ptr2
            tax
            inx
            stx     ptr2
            jsr     popptr1         ; Buffer address in ptr1
            jsr     popax           ; fd in A
            sta     tmp1            ; fd in tmp1
            cmp     #STDOUT_FILENO  ; Is this for STDOUT?
            beq     begin           ; Taken if SO
            cmp     #STDERR_FILENO  ; Perhaps this is for STDERR?
            bne     fbegin          ; Taken if NOT

; STDOUT and STDERR, console output routine
begin:      dec     ptr2
            bne     L1
            dec     ptr2+1
            beq     done

L1:         ldy     #0
            lda     (ptr1),y

; The Neo6502 kernal interprets '\r' as  \r\n so skip '\n'.
            cmp     #$0A              ; Check for '\n'.
            bne     chout             ; 
            lda     #$0D              ; replace \n with \r which produces \r\n

chout:      sta API_PARAM0            ; the char to write
            lda #API_FN_WRITE_CHAR    ; Using X-reg because A has the char to write
            sta API_FUNCTION
            lda #API_GROUP_CONSOLE
            sta API_COMMAND           ; Make it so.
; uncomment to enable error check for every console write:
;***        lda API_ERROR             ; How did it go?
;***        bne conerr                ; Taken if error

next:       inc     ptr1
            bne     begin
            inc     ptr1+1
            jmp     begin

done:       lda     ptr3
            ldx     ptr3+1
            rts                     ; Return count

; console write error exit:
conerr:     lda #EIO                 ; Generic IO error
            jsr ___seterrno
            lda #$ff    ; error return
            ldx #$ff
            rts 
;-----------------------------
; FILE output routine
; fd in A-reg and tmp1
; count in ptr3
; buffer address in ptr1
;-----------------------------
fbegin:     sta API_PARAM0      ; fd
; check if this stream has WRONLY set:
            tay                 ; fd
            lda flags_tab,y     ; Get file open flags for this fd
            and #O_WRONLY       ; can this stream be written?
            beq ferr            ; taken if NOT (not a writeablestream) 
; regular processing:
            lda ptr1            ; buffer pointer
            sta API_PARAM1
            lda ptr1+1
            sta API_PARAM2
            lda ptr3            ; size
            sta API_PARAM3
            lda ptr3+1
            sta API_PARAM4
            lda #API_FN_FILE_WRITE
            sta API_FUNCTION
            lda #API_GROUP_FILEIO
            sta API_COMMAND     ; Make it so.
            lda API_ERROR       ; How did it go?
            bne ferr            ; Taken if error
fdone:      lda API_PARAM3      ; bytes written
            ldx API_PARAM4
            rts

; Attempt to write a read-only stream.
 ferr:      lda #EACCES         ; Access denied
            jsr ___seterrno
            lda #$ff    ; error return
            ldx #$ff
            rts    

.endproc

