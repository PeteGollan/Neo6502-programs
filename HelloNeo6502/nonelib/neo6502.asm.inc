; Convenience macros for Neo6502 applications programming
; SPDX-License-Identifier: CC0-1.0


;-----------------------------------------------------;
; Neo6502 Kernel jump vectors (see kernel/kernel.asm) ;
;-----------------------------------------------------;

ReadLine       = $FFEB
ReadCharacter  = $FFEE
WriteCharacter = $FFF1
WaitMessage    = $FFF4
SendMessage    = $FFF7


;--------------------------------------;
; Neo6502 Kernel API control addresses ;
;--------------------------------------;

ControlPort = $FF00
API_COMMAND    = ControlPort + 0 ; function group address
API_FUNCTION   = ControlPort + 1 ; function address
API_ERROR      = ControlPort + 2 ; function error codes
API_STATUS     = ControlPort + 3 ; misc hardware status codes (bit-field)
API_PARAMETERS = ControlPort + 4 ; function parameters base address (+0-7)

