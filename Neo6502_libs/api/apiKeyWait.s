;---------------------------------------------------------------------
; apiKeyWait.s - unsigned char __fastcall__ apiKeyWait();
;
; Waits on API CONSOLE READ CHARACTER for a key press and then returns
; its ASCII value. Good for "Press RETURN..." or "Press any key..."
; as long as function keys, numpad keys, and other special keys are
; not required.
;
; Note:
;   The Neo6502 READ_CHAR function returns the ENTER key as 0x0D. We
;   detect this and substitute 0x0A, which is the correct ASCII code
;   for ENTER. The console section of read() does the same substituion.
;   Furthermore, many of the CC65 target read() functions do the same
;   (e.g. kim1).
;
; By Pete Wyspianski (aka Gollan), September 2024
;---------------------------------------------------------------------

        .include   "zeropage.inc"
        .include   "neo6502.asm.inc"

        .export     _apiKeyWait

        .importzp   ptr1

.proc _apiKeyWait

; Set up the CONSOLE Read Character call:
            lda #API_FN_READ_CHAR 
            sta API_FUNCTION
char_wait:  lda #API_GROUP_CONSOLE
            sta API_COMMAND      ; Make it so.
            lda API_PARAM0
            beq char_wait       ; zero means no character
            cmp #$0D            ; Did they press ENTER on the keyboard?
            bne no_adj          ; Take if NOT
            lda #$0A            ; We know that you meant ASCII ENTER.
no_adj:     ldx #0              ; return value in AX
            rts
.endproc
