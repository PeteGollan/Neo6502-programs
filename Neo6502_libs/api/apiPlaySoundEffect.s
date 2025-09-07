;--------------------------------------------------------------------
; apiPlaySoundEffect.s - void __fastcall__ apiPlaySoundEffect(uint8_t sound_ch, uint8_t sound_effect);
;
; Plays a sound effect on the specified channel.
;
; By Pete Wyspianski (aka Gollan), August 2025
;--------------------------------------------------------------------

        .include   "zeropage.inc"
        .include   "neo6502.asm.inc"

;       .import popax
        .import popa

        .export     _apiPlaySoundEffect

.proc _apiPlaySoundEffect

; On entry, A = sound_effect (8 bits)
            sta API_PARAM1
; Get sound_ch in A:
            jsr popa            ; NOTE: this is "popa" NOT "popax"
            sta API_PARAM0
; Set up the SOUND group API_FN_PLAY_SOUND call:
            lda #API_FN_PLAY_SOUND
            sta API_FUNCTION
            lda #API_GROUP_SOUND
            sta API_COMMAND      ; Make it so.
            rts
.endproc

