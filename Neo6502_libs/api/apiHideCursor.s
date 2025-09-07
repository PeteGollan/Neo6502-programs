;---------------------------------------------------------------------
; apiHideCursor.s - void __fastcall__ HideCursor(void);
;
; Shows the console cursor.
;
; By Pete Wyspianski (aka Gollan), September 2024
;---------------------------------------------------------------------

        .include   "zeropage.inc"
        .include   "neo6502.asm.inc"

        .export     _apiHideCursor

        .importzp   ptr1

.proc _apiHideCursor

; Set up the CONSOLE SHOW/HIDE Cursor call:
            lda #API_FN_SHOW_HIDE_CURSOR
            sta API_FUNCTION
            lda #0      ; 0 = hide cursor
            sta API_PARAM0
            lda #API_GROUP_CONSOLE
            sta API_COMMAND      ; Make it so.
            rts
.endproc
