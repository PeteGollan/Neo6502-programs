; ---------------------------------------------------------------------------
; crt0.s for CC65->Neo6502
;
; Revised by Pete Wyspianski (aka Gollan), June 2024
; This version of crt0 returns to NeoBASIC via the API call upon exit.
; ---------------------------------------------------------------------------

.export _init , _exit
.import _main

.export __STARTUP__ : absolute = 1   ; Mark as startup
.import __RAM_START__ , __RAM_SIZE__ ; Linker generated
.import __STACKSIZE__

.import copydata , zerobss , initlib , donelib

.include   "neo6502.asm.inc"
.include  "zeropage.inc"

; ---------------------------------------------------------------------------
; Place the startup code in a special segment

.segment  "STARTUP"


  ; ---------------------------------------------------------------------------
  ; A little light 6502 housekeeping

  ;*********************************** 
_init:
    cld      ; Clear decimal mode

  ; ---------------------------------------------------------------------------
  ; Set cc65 argument stack pointer

  lda #<(__RAM_START__ + __RAM_SIZE__  + __STACKSIZE__)
  ldx #>(__RAM_START__ + __RAM_SIZE__  + __STACKSIZE__)
  sta sp
  stx sp+1


  ; ---------------------------------------------------------------------------
  ; Initialize memory storage

  jsr zerobss  ; Clear BSS segment
  jsr copydata ; initialise data segment
  jsr initlib  ; Run constructors


  ; ---------------------------------------------------------------------------
  ; Call main()

  jsr _main


  ; ---------------------------------------------------------------------------
  ; Back from main (this is also the _exit entry):

_exit:
  jsr donelib ; Run destructors


; return to NeoBASIC
_endx:

; Cleanly exit via the API call:

            lda #API_FN_BASIC
            sta API_FUNCTION
            lda #API_GROUP_SYSTEM
            sta API_COMMAND     ; Make it so.
            jmp ($00)           ; Vector to NeoBASIC
