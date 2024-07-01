;
; int __fastcall__ write (int fd, const void* buf, int count);
;
; Adapted from kim1 version by Pete Wyspianski, June 2024.

    .include        "neo6502.asm.inc"

    .export         _write
    .import         popax, popptr1
    .importzp       ptr1, ptr2, ptr3


;.segment "CODE"

 ;.proc  _write
 _write:
        sta     ptr3
        stx     ptr3+1          ; Count in ptr3
        inx
        stx     ptr2+1          ; Increment and store in ptr2
        tax
        inx
        stx     ptr2
        jsr     popptr1         ; Buffer address in ptr1
        jsr     popax

begin:  dec     ptr2
        bne     L1
        dec     ptr2+1
        beq     done

L1:     ldy     #0
        lda     (ptr1),y
        pha                     ; Save A (changed by OUTCHR)
; The Neo6502 kernal interprets '\r' as  \r\n so skip '\n'.
        cmp     #$0A            ; Check for '\n'.
        bne     chout             ; 
        lda     #$0D            ; replace \n with \r which produces \r\n
chout:  jsr     WriteCharacter  ; Send character using Monitor call
        pla                     ; Restore A

next:   inc     ptr1
        bne     begin
        inc     ptr1+1
        jmp     begin

done:   lda     ptr3
        ldx     ptr3+1
        rts                     ; Return count
;.endproc

