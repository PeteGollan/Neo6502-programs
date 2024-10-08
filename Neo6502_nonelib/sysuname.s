;
; Ullrich von Bassewitz, 2003-08-12
;
; Adapted for Neo6502 from the Telestrat version
; by Pete Wyspianski (aka Gollan), August 2024
;
; unsigned char __fastcall__ _sysuname (struct utsname* buf);
;

        .export         __sysuname, utsdata

        .import         utscopy

        __sysuname = utscopy

;--------------------------------------------------------------------------
; Data. We define a fixed utsname struct here and just copy it.

.rodata

utsdata:
        ; sysname
        .asciiz         "cc65"

        ; nodename
        .asciiz         ""

        ; release
        .byte           ((.VERSION >> 8) & $0F) + '0'
        .byte           '.'
        .if             ((.VERSION >> 4) & $0F) > 9
        .byte           ((.VERSION >> 4) & $0F) / 10 + '0'
        .byte           ((.VERSION >> 4) & $0F) .MOD 10 + '0'
        .else
        .byte           ((.VERSION >> 4) & $0F) + '0'
        .endif
        .byte           $00

        ; version
        .if             (.VERSION & $0F) > 9
        .byte           (.VERSION & $0F) / 10 + '0'
        .byte           (.VERSION & $0F) .MOD 10 + '0'
        .else
        .byte           (.VERSION & $0F) + '0'
        .endif
        .byte           $00

        ; machine
        .asciiz         "Neo6502"
