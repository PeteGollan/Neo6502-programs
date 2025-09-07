;--------------------------------------------------------------------
; apiSetInkPaperColour.s - void __fastcall__ apiSetInkPaperColour(uint8_t fg, uint8_t bg);
;Note: 
; Sets the text and background colours. Input values are in the range 0-15.
; Notes: Does NOT range check the parameters.
;        This implements the SetTextColor(), but uses a symetrical name with
;        apiGetInkPaperColour(). Alternate names are provided in the header file.
;
; By Pete Wyspianski (aka Gollan), September 2025
;--------------------------------------------------------------------

        .include   "zeropage.inc"
        .include   "neo6502.asm.inc"

;       .import popax
        .import popa

        .export     _apiSetInkPaperColour

.proc _apiSetInkPaperColour

; On entry, A = bg (8 bits)
            sta API_PARAM1
; Get fg in A:
            jsr popa            ; NOTE: this is "popa" NOT "popax"
            sta API_PARAM0
; Set up the get CONSOLE FN_API_FN_SET_CURSOR_POS call:
            lda #API_FN_SET_TEXT_COLOUR
            sta API_FUNCTION
            lda #API_GROUP_CONSOLE
            sta API_COMMAND      ; Make it so.
            rts
.endproc
