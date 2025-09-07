;--------------------------------------------------------------------
; apiGetInkPaperColour.s - void __fastcall__ apiGetInkPaperColour(uint8_t* fg, uint8_t* bg);
;
; Takes pointers to uint8_t for the foreground (ink) and background (paper) colours.
; Returns colour values in the range 0-15. 
;
; By Pete Wyspianski (aka Gollan), September 2024
;
; Sept 2025 Clarified the return value range.
;--------------------------------------------------------------------

        .include   "zeropage.inc"
        .include   "neo6502.asm.inc"

        .import popax

        .export     _apiGetInkPaperColour

        .importzp   ptr1,ptr2

.proc _apiGetInkPaperColour

; On entry, AX = pointer to bg 
            sta ptr2
            stx ptr2+1
; Get pointer to fg in AX:
            jsr popax
            sta ptr1
            stx ptr1+1

; Set up the get CONSOLE FN_GET_INK_PAPER_COLOUR call:
            lda #API_FN_GET_INK_PAPER_COLOUR
            sta API_FUNCTION
            lda #API_GROUP_CONSOLE
            sta API_COMMAND      ; Make it so.
; Copy the two 1-byte values to their respective variables:
            lda API_PARAM0
            sta (ptr1)
            lda API_PARAM1
            sta (ptr2)
            rts
.endproc
