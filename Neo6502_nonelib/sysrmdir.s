;---------------------------------------------------------------------------
; sysrmdir.s -  int __fastcall__ __sysrmdir(const char* name); 
;
; By Pete Wyspianski (aka Gollan), August 2024
;
; NOTES:
;  Neo6502 API max filename length is 255 chars + 1 length byte:
;  This function returns system error code in AX, which is the requirement
;  for _sys*** functions. seterrno will be called by our caller, not us.
;---------------------------------------------------------------------------

    .export    __sysrmdir
    .import    ___seterrno
    .import    _makebstr, _freebstr

    .import    addysp,popax

; NOTE: other CC65 functions may use these (e.g. malloc()):
    .importzp  sp,ptr1,ptr2,tmp1,tmp2,tmp3

    .include   "errno.inc"
    .include   "fcntl.inc"
    .include   "neo6502.asm.inc"

.proc __sysrmdir

; On entry, AX is a pointer to our null-terminated file name
; Copy null-terminated file name to length-prefixed string in a new memory block.
            jsr _makebstr
            cpy #0              ; good return?
            beq mbs_ok1         ; taken if so
;_makebstr error exit. A-reg-error code X-reg zero
            rts                 ; AX error code set by _makebstr
;
mbs_ok1:    sta ptr2            ; Save file name buffer
            stx ptr2+1          ; ptr2 points at the future length byte
; See if the target name exists:
            sta API_PARAM0      ; set up the call to FILE STAT
            stx API_PARAM1
            lda #API_FN_FILE_STAT
            sta API_FUNCTION
            lda #API_GROUP_FILEIO
            sta API_COMMAND     ; Make it so.
            lda API_ERROR       ; How did it go? 0 = file exists
            beq name_found      ; taken if target name exists...
; The name doesn't exist, so clean up and exit:
            lda ptr2            ; return any mallocs we had
            ldx ptr2+1
            jsr _freebstr 
            lda #ENOENT         ; "No such file or directory" is a good match.
            ldx #0
            rts   
; The target name exists, is it a directory?
name_found: lda API_PARAM4      ; attribute bits
            and #$01            ; see if bit 0 is set (directory)
            bne rm_dir          ; taken if target is a directory
; A file (not directory) with the given name exists, so clean up and exit.
            lda ptr2            ; return any mallocs we had
            ldx ptr2+1
            jsr _freebstr 
; The error is rmdir() target was a file, not a directory:
            lda #EINVAL        ; "Invalid argument" is as close as we have.
            ldx #0
            rts                 ; error exit
; Remove the directory with the Neo6502 API (FILE_DELETE is used for both files and dirs):
 rm_dir:    lda ptr2            ; Get the directory name
            ldx ptr2+1
            sta API_PARAM0      ; length-prefixed filename string...
            stx API_PARAM1      ; ... to PARAM0, PARAM1
            lda #API_FN_FILE_DELETE
            sta API_FUNCTION
            lda #API_GROUP_FILEIO
            sta API_COMMAND     ; Make it so.
; We will check for errors after returning the filename buffer:
            lda ptr2            ; memory block, pointer in AX
            ldx ptr2+1
            jsr _freebstr
            lda API_ERROR       ; How did it go?
            beq exit_ok         ; zero means success.
; The most likely error is that the directory is not empty
; as we know the directory exists:
            lda #EACCES         ; permission denied is as close as we have
            ldx #$0
            rts
; Done:
exit_ok:    lda #$00    ; 0 = success return
            ldx #$00
            rts

.endproc