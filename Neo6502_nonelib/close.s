;----------------------------------------------------
; close.s - int close(int fd); 
; jede jede@oric.org 2017-01-22
;
; Adapted for Neo6502 from the Telestrat version
; by Pete Wyspianski (aka Gollan), July 2024
;-----------------------------------------------------
;

        .export         _close

        .import     addysp,popax
        .import     neo6502_fd,mask_tab  ; from open.s
        .import     ___seterrno
        .importzp  tmp1

        .include   "zeropage.inc"
        .include   "errno.inc"
        .include   "neo6502.asm.inc"

; int __fastcall__ close (int fd);
.proc _close

; On entry, A = fd
; If we are being called by freopen() the fd could be <=3. In that case,
; just return success:
            cmp #$03             ; One of the special fds? (1-3)
            bcc exit_ok           ; taken if so
; free the fd:
free_fd:    sta tmp1             ; save fd to use for the API call
            tay                  ; fd to y
            dey                  ; subtract the 3 offset from Y...
            dey                  ; ... only for table access
            dey
            lda mask_tab,y       ; get the mask for this fd
            eor #$ff             ; complement the mask
            and neo6502_fd       ; clear the target bit (free that fd)
            sta neo6502_fd

; close the file with the Neo6502 API

            lda #API_FN_FILE_CLOSE
            sta API_FUNCTION
            lda tmp1             ; fd
            sta API_PARAMETERS   ; param 0 = fd 
            lda #API_GROUP_FILEIO
            sta API_COMMAND      ; Make it so.
            lda API_ERROR        ; How did it go?
            bne err              ; non-zero means error.
exit_ok:    lda #$00
            ldx #$00
            rts

; Error exit. Try to inform what happened.
;
err:        cmp #$01             ; Bad fd?
            bne err_unk          ; taken if not
            lda #EBADF           ; Bad file number
            jsr ___seterrno
            bra err_exit
err_unk:    lda #EUNKNOWN        ; unknown error (could add more checks here)
            jsr ___seterrno
err_exit:   lda #$ff             ; error return
            ldx #$ff
            rts
.endproc
