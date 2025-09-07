;---------------------------------------------------------------------
; apiRand16.s - uint16_t __fastcall__ apiRand16(void);
;
; Returns a 16-bit random integer in the range 0-65535,
; inclusive, using the Neo6502 API Random Integer function.
;
; By Pete Wyspianski (aka Gollan), September 2024
;---------------------------------------------------------------------

        .include   "zeropage.inc"
        .include   "neo6502.asm.inc"

        .export     _apiRand16

.proc _apiRand16

; Set up the MATH RANDOM INTEGER call:

  ; set max range of requested random integer... 5 bytes, every second address
            lda #$00
            sta reg1+0          ; 00 type: 32-bit integer, little endian
            sta reg1+2          ; 00 byte0 (lsb) of max range+2 $ffff+1/65535+1 (only asking for 16-bits)
            sta reg1+4          ; 00 byte1 of max range
            lda #$01
            sta reg1+6          ; 01 byte2 of max range
            lda #$00
            sta reg1+8;         ; 00 byte3 (msb) of max range

            lda #<reg1          ; LSB of Reg1
            sta API_PARAM0
            lda #>reg1          ; MSB of Reg1
            sta API_PARAM1
            lda #2              ; Step between bytes of each register
            sta API_PARAM2
            lda #API_FN_RANDOM_INTEGER
            sta API_FUNCTION
lda #API_GROUP_MATH
            sta API_COMMAND     ; Make it so.
            lda reg1+2          ; lsb
            ldx reg1+4          ; msb
            rts
.endproc

; This *IS* as confusing as it looks. The two math registers are inteleaved.
; Integers are stored in little-endian order, with byte zero being the "type byte".
reg1:       .byte $a5    ; reg1[0] - type byte: zero for integer
reg2:       .byte $a5
            .byte $a5    ; reg1[1]
            .byte $a5    ; 
            .byte $a5    ; reg1[2]
            .byte $a5
            .byte $a5    ; reg1[3]
            .byte $a5
            .byte $a5    ; reg1[4]
            .byte $a5
            .byte $a5
