;--------------------------------------------------------------------
; apiSetCursorPos.s - void __fastcall__ apiSetCursorPos(uint8_t x_pos, uint8_t y_pos);
;
; Sets the text cursor position to the given values.
; Note: Does NOT range check the parameters.
;
; By Pete Wyspianski (aka Gollan), September 2024
;--------------------------------------------------------------------

        .include   "zeropage.inc"
        .include   "neo6502.asm.inc"

;       .import popax
        .import popa

        .export     _apiSetCursorPos

.proc _apiSetCursorPos

; On entry, A = y_pos (8 bits)
            sta API_PARAM1
; Get x_pos in A:
            jsr popa            ; NOTE: this is "popa" NOT "popax"
            sta API_PARAM0
; Set up the get CONSOLE FN_API_FN_SET_CURSOR_POS call:
            lda #API_FN_SET_CURSOR_POS
            sta API_FUNCTION
            lda #API_GROUP_CONSOLE
            sta API_COMMAND      ; Make it so.
            rts
.endproc
