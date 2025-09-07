;--------------------------------------------------------------------
; apiDelay10ms.s -  void __fastcall__ apiDelay10ms(uint16_t t);
;
; By Pete Wyspianski (aka Gollan), August 2024
;
; Waits for the specified number of 10ms intervals
; * Uses the 100Hz system timer function
; * t=0 returns immediately
; * t=100 is 1 sec, t=1000 is 10 sec, max t=65535 is 655 sec (10m 55sec)
; * Due to the polling frequency, there is no issue with 100Hz counter
;   roll-over.
;--------------------------------------------------------------------

        .include   "neo6502.asm.inc"

        .export     _apiDelay10ms

.proc _apiDelay10ms

; On entry, AX = the time parameter
            cpx #0
            bne start
            cmp #0      ; check lsb of t param
            bne start
            rts         ; t = 0 so bounce right back
; Get the initial SYSTEM TIMER value:
start:      tay         ; save LSB in y
            lda #API_FN_TIMER
            sta API_FUNCTION
            lda #API_GROUP_SYSTEM
            sta API_COMMAND      ; Make it so.
; Now there are 4 bytes of timer value in PARAM0-3
; We will only work with the two least-significant bytes.
; Add the t parameter to the current timer value:
            tya         ; a-reg = t
            clc
            adc API_PARAM0
            sta end_time+0
            txa
            adc API_PARAM1
            sta end_time+1

; We have the target time set, now wait for the system 100Hz timer
; to equal or exceed that number.

check_time: lda #API_FN_TIMER
            sta API_FUNCTION
            lda #API_GROUP_SYSTEM
            sta API_COMMAND      ; Make it so.
            lda API_PARAM1
            cmp end_time+1
            bcc check_time      ; new time is < target, so continue checking
            lda API_PARAM0
            cmp end_time+0
            bcc check_time      ; new time is < target, so continue checking
            rts

.endproc

end_time:   .byte 00 
            .byte 00
            .byte 00
            .byte 00
