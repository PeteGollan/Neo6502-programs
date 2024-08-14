;-------------------------------------------------------------------
; lseek.s 
;   off_t __fastcall__ lseek(int fd, off_t offset, int whence);
;
; Jede (jede@oric.org), 2023-03-13
;
; Adapted from the Telestrat version
; by Pete Wyspianski (aka Gollan), July 2024
;------------------------------------------------------------------


        .export         _lseek
        .include   "stdio.inc"
        .include   "errno.inc"
        .include   "neo6502.asm.inc"
        .include   "zeropage.inc"

        .import     popax
        .importzp   tmp1, tmp2, tmp3, tmp4

.proc _lseek

; whence is in AX on entry:
            sta tmp1          ; whence (just a flag so keep lsb)

; offset is 32-bit value: aabbccdd
; ccdd are on stack, followed by aabb (which are also in sreg)
; Neo6502 API is little-endian, so lsbs first:

            jsr popax       ; two lsbs of offset in AX
            sta API_PARAM1
            stx API_PARAM2 
            jsr popax       ; two msbs of offset in AX
            sta API_PARAM3
            stx API_PARAM4

            jsr popax           ; fd in AX 
            sta API_PARAM0      ; fd (only need lsb)

; The Neo6502 API File Seek function only does "SEEK_SET",
; so other "whence" modes will require additional processing.
;
; Possible whence modes are: SEEK_SET, SEEK_CUR, SEEK_END
 
            lda tmp1        ; whence
            cmp #SEEK_SET
            beq do_seek_set ; call the API as-is
            cmp #SEEK_CUR
            beq do_seek_cur
            cmp #SEEK_END
            beq do_seek_end

; Bad whence parameter (calling function does NOT check):
bad_whence: lda #EINVAL      ; Invalid argument
            jmp ferr1        ; error exit

;------------------------------------------------------------------------------
; SEEK_END - offset is relative to the end of the file.
; Identical to SEEK_CUR except instead of getting the current file position
; into API_PARAM1-4, we get the file size (i.e. the position of the end of the file)
;------------------------------------------------------------------------------
do_seek_end: lda #API_FN_FILE_SIZE
             bra do_seek1
;------------------------------------------------------------------------------
; SEEK_CUR - offset is relative to current position.
; Save offset, call ftell() to populate param 1-4 with the current
; position, and then add offset to the current position.
; ftell() doesn't require an special handling, as offset is zero.
;------------------------------------------------------------------------------

do_seek_cur:
            lda #API_FN_FILE_TELL
; entry point for SEEK_END:
do_seek1:   sta API_FUNCTION       ; either FILE_TELL or FILE_SIZE
            lda API_PARAM1 ; save offset
            sta tmp1
            lda API_PARAM2
            sta tmp2
            lda API_PARAM3
            sta tmp3
            lda API_PARAM4
            sta tmp4

; fd is already in API_PARAM0
; API_FUNCTION is already set   ; either FILE_TELL or FILE_SIZE
            lda #API_GROUP_FILEIO
            sta API_COMMAND     ; Make it so.
            lda API_ERROR       ; How did it go?
            bne ferr            ; Taken if error
; now we have the CURRENT offset in API_PARAM1-4, so add the offset:
            clc
            lda    API_PARAM1    ; start with LSB
            adc    tmp1
            sta    API_PARAM1
            lda    API_PARAM2
            adc    tmp2
            sta    API_PARAM2
            lda    API_PARAM3
            adc    tmp3
            sta    API_PARAM3
            lda    API_PARAM4    ; end with MSB
            adc    tmp4
            sta    API_PARAM4
; should look for overflow or undeflow here
; can't just use carry because offset can be negative

do_seek_set:

;---------------------------------------------------
; fseek()
; fd is already in API_PARAM0
; offset is already in API_PARAM1-4
;---------------------------------------------------
do_fseek:
; At this point, the seek position is absolute. 
; Screen out a nonsensical seek position:
            lda API_PARAM4      ; get the msb
            bmi ferr            ; bail out if it is negative
            lda #API_FN_FILE_SEEK 
            sta API_FUNCTION
            lda #API_GROUP_FILEIO
            sta API_COMMAND     ; Make it so.
            lda API_ERROR       ; How did it go?
            bne ferr            ; Taken if error
;... falls into ftell()

;---------------------------------------------------
; ftell()
; fd is already in API_PARAM0
;---------------------------------------------------
do_ftell:   lda #API_FN_FILE_TELL
            sta API_FUNCTION
            lda #API_GROUP_FILEIO
            sta API_COMMAND     ; Make it so.
            lda API_ERROR       ; How did it go?
            bne ferr            ; Taken if error
; return a 32-bit (4 byte) long int in sreg and AX:
ret_foffset:
            lda API_PARAM4      ; msb
            sta sreg+1
            lda API_PARAM3        ; next highest byte
            sta sreg
            ldx API_PARAM2        ; next lowest 
            lda API_PARAM1        ; lsb
            rts

; Report error as best we can:
 ferr:     lda #ESPIPE     ; Illegal seek
 ferr1:    jsr ___seterrno
;returning a 32-bit (4 byte) long (in this case -1):
           lda #$ff       ; lsb
           tax            ; next lowest byte
           sta sreg       
           sta sreg+1     ; next highest byte
           rts
.endproc
