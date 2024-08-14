;--------------------------------------------------------------------
; oserror.s
;
; Jede, 2017-10-27
;
; Adapted for Neo6502 from telestrat version
; by Pete Wyspianski (aka Gollan), July 2024
;
; For Neo6502, we just return the same error code, because our drivers
; already return the "errno.inc" error codes.
;--------------------------------------------------------------------

; int __fastcall__ __osmaperrno (unsigned char oserror);
; /* Map a system specific error into a system independent code */
;

        .include        "errno.inc"
        .export         ___osmaperrno

.proc   ___osmaperrno

        rts

.endproc
