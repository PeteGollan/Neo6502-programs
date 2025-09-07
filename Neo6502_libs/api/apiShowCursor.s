;---------------------------------------------------------------------
; apiShowCursor.s - void __fastcall__ ShowCursor(void);
;
; Shows the console cursor.
;
; By Pete Wyspianski (aka Gollan), September 2024
;---------------------------------------------------------------------

        .include   "zeropage.inc"
        .include   "neo6502.asm.inc"
 
        .export     _apiShowCursor

        .importzp   ptr1

.proc _apiShowCursor

; Set up the CONSOLE SHOW/HIDE Cursor call:
            lda #API_FN_SHOW_HIDE_CURSOR
            sta API_FUNCTION
            lda #1      ; 1 = show cursor
            sta API_PARAM0
            lda #API_GROUP_CONSOLE
            sta API_COMMAND      ; Make it so.
            rts
.endproc
