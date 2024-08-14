;-------------------------------------------------------------------------------
; sysmkdir.s
;   -  int __sysmkdir(const char* name, ...);
;      /* May take a mode argument */
;
; By Pete Wyspianski (aka Gollan), August2024
;
; NOTES:
;    * Neo6502 API max filename length is 255 chars + 1 length byte:
;    * This function returns system error code in AX, which is the requirement
;      for _sys*** functions. seterrno will be called by our caller, not us.
;    * Silently ignores any "mode" argument.
;    * Checks and returns an error if the name is already in use (dir or file).
;-------------------------------------------------------------------------------

    .export    __sysmkdir
    .import    ___seterrno
    .import    _makebstr, _freebstr

    .import    addysp,popax

; NOTE: other CC65 functions may use these (e.g. malloc()):
    .importzp  sp,ptr1,ptr2,tmp1,tmp2,tmp3

    .include   "errno.inc"
    .include   "fcntl.inc"
    .include   "neo6502.asm.inc"

.proc __sysmkdir
; 
; Throw away any additional parameters passed through the ellipsis (e.g. a mode specifier)
            dey                 ; Expecting a pointer to the filename (2 bytes)
            dey
            beq parmok          ; Branch if parameter count ok
            jsr addysp          ; Fix stack, throw away unused parameters
parmok:     jsr popax           ; Get filename pointer
; Copy null-terminated string to length-prefixed string in a new memory block.
; AX has open file name ptr.
            jsr _makebstr
            cpy #0              ; good return?
            beq mbs_ok          ; taken if so
;_makebstr error exit. Errors are already handled and AX has error code.
            rts

mbs_ok:     sta ptr2            ; Save filename pointer
            stx ptr2+1
; ptr to length-prefixed filename is AX
; See if the target name is already in use:
            sta API_PARAM0      ; length-prefixed filename string...
            stx API_PARAM1      ; ... to PARAM0, PARAM1
            lda #API_FN_FILE_STAT
            sta API_FUNCTION
            lda #API_GROUP_FILEIO
            sta API_COMMAND     ; Make it so.
            lda API_ERROR       ; How did it go? 0 = name (file or dir) exists
            bne mk_dir          ; "failure" means the name doesn't exist...
; At this point, the call has failed (the name exists, either a file or directory)
; so clean up and get out of Dodge:
            lda ptr2            ; return any mallocs we had
            ldx ptr2+1
            jsr _freebstr
; return an error code (NOT -1) 
            lda #EEXIST         ; File already exists (not super accurate but OK)
            ldx #0
            rts                 ; error exit 

; The target name is available, so make the directory:
mk_dir:     lda ptr2            ; Get the file name
            ldx ptr2+1
            sta API_PARAM0    
            stx API_PARAM1  
            lda #API_FN_DIR_MKDIR
            sta API_FUNCTION
            lda #API_GROUP_FILEIO
            sta API_COMMAND     ; Make it so.
; We will check for errors after returning the filename buffer:
            lda ptr2            ; memory block, pointer in AX
            ldx ptr2+1
            jsr _freebstr
            lda API_ERROR       ; How did it go?
            beq exit_ok         ; zero means success.
            lda #EUNKNOWN       ; unknown error 
            ldx #$0
            rts

; WE are done
exit_ok:    lda #$00    ; 0 = success return
            ldx #$00
            rts

.endproc
