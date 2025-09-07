;--------------------------------------------------------------------
; apiGetSysTimer16.s  uint16_t __fastcall__ apiGetSysTimer16(void);
;
; By Pete Wyspianski (aka Gollan), September 2024
;
; Returns the two least significant bytes of the 100Hz system timer.
;--------------------------------------------------------------------

        .include   "neo6502.asm.inc"

        .export     _apiGetSysTimer16

.proc _apiGetSysTimer16

; Get the SYSTEM TIMER value:
            lda #API_FN_TIMER
            sta API_FUNCTION
            lda #API_GROUP_SYSTEM
            sta API_COMMAND      ; Make it so.
; Now there are 4 bytes of timer value in PARAM0-3
; Return only the two LSBs in AX:
            lda API_PARAM0
            ldx API_PARAM1
            rts
.endproc

