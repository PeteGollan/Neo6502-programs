;-----------------------------------------------------------------
; get_morpheus_vers.s 
;   int __fastcall__ _get_morpheus_vers(morpheus_vers *mv);
;
; By Pete Wyspianski (aka Gollan), August 2024
;-----------------------------------------------------------------

    .export    _get_morpheus_vers
    
    .importzp  ptr1
    
    .include   "neo6502.asm.inc"

.proc _get_morpheus_vers

; On entry, AX is a pointer to our morpheus_vers structure
            sta ptr1
            stx ptr1+1
            lda #API_FN_API_VERS 
            sta API_FUNCTION
            lda #API_GROUP_SYSTEM 
            sta API_COMMAND     ; Make it so.
            lda API_ERROR       ; How did it go?
            beq copy_data       ; zero means success.
; something went wrong so:
            lda #$ff            ; -1 = error returns
            ldx #$ff
            rts
; copy the parameters over:
copy_data:    lda API_PARAM0
            ldy #_morpheus_vers::major
            sta (ptr1),y
            lda API_PARAM1
            ldy #_morpheus_vers::minor
            sta (ptr1),y
            lda API_PARAM2
            ldy #_morpheus_vers::patch
            sta (ptr1),y
; WE are done, return filename buffer and exit:
exit_ok:    lda #$0    ; 0 = success return
            ldx #$0
            rts

.endproc
