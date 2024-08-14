;----------------------------------------------------------------------------------------
; sysrename.s - int __fastcall__ __sysrename(const char* oldname, const char* newname);
;
; By Pete Wyspianski (aka Gollan), July 2024
;
; NOTE:
;  * The Neo6502 API maximum filename length is 255 characters.
;  * This function returns system error code in AX, which is the requirement
;    for _sys*** functions. seterrno will be called by our caller, not us.
;----------------------------------------------------------------------------------------

    .export    __sysrename
    .import    _makebstr, _freebstr
    .import    lptr1, lptr2        ; local pointers for us to use in open.s

    .import    addysp,popax,popptr1
    
; NOTE: *** Other CC65 functions may use these (e.g. _malloc):
    .importzp  sp,ptr1,ptr2,tmp1,tmp2,tmp3

    .include   "errno.inc"
    .include   "fcntl.inc"
    .include   "neo6502.asm.inc"

.proc __sysrename

; On entry, AX is a pointer to our null-terminated NEW file name 
; and the stack contains a pointer to the OLD file name.
; Copy null-terminated file name to length-prefixed string in a new memory block.
            jsr _makebstr
            cpy #0              ; good return?
            beq mbs_ok1         ; taken if so
;_makebstr error exit. Errors are already handled. A-error code, X-zero
mbs_err:    rts                 ; error return

; The first _makebstr succeeded, save the buffer pointer:
mbs_ok1:    sta lptr2           ; pointer to buffer for NEW name...
            stx lptr2+1         ; in b-string format
; Get the pointer the null-terminated OLD filename:
            jsr popax           ; AX is pointer to OLD file name
            jsr _makebstr
            cpy #0              ; good return?
            beq mbs_ok2         ; taken if so
; _makebstr error exit. Errors are already handled for the current string,
; but we have to free the previous one:
            pha                 ; save the error code returned by makebstr
            phx
            lda lptr2
            ldx lptr2+1          
            jsr _freebstr
            plx
            pla                 ; get the error codoe
            rts                 ; return A-error code, X-zero

; both file name strings are copied.
mbs_ok2:
            sta lptr1           ; pointer to buffer for OLD name...
            stx lptr1+1         ; in b-string format

; The Neo6502 API FILE RENAME has an issue to be addressed here.
; If a file already exists with the new name, the call will silently wipe out
; that file. Instead, let's use FILE STAT to check if the new file name exists
; (file or dir) and return a helpful error if so:

            lda lptr2           ; pointer to buffer for NEW name
            ldx lptr2+1
            sta API_PARAM0      ; set up the call to FILE STAT
            stx API_PARAM1
            lda #API_FN_FILE_STAT
            sta API_FUNCTION
            lda #API_GROUP_FILEIO
            sta API_COMMAND     ; Make it so.
            lda API_ERROR       ; How did it go? 0 = file exists
            bne ok_to_rn        ; "failure" means the new file doesn't exist...
; A file or dir with the new file name already exists, so clean up and exit:

            lda lptr1           ; return any mallocs we had
            ldx lptr1+1
            jsr _freebstr       ; no harm if they are null
            lda lptr2
            ldx lptr2+1
            jsr _freebstr       ; no harm if they are null
            lda #EEXIST         ; return an error code (NOT -1)
            ldx #0
            rts                 ; exit 
ok_to_rn:   lda lptr1           ; pointer to buffer for OLD name
            sta API_PARAM0      ; API_PARAM0,1 OLD name
            ldx lptr1+1
            stx API_PARAM1
            lda lptr2           ; pointer to buffer for NEW name
            sta API_PARAM2      ; API_PARAM2,3 NEW name
            ldx lptr2+1
            stx API_PARAM3
; Rename the file with the Neo6502 API
            lda #API_FN_FILE_RENAME
            sta API_FUNCTION
            lda #API_GROUP_FILEIO
            sta API_COMMAND      ; Make it so.
; We will check for errors after returning the filename buffers:
            lda lptr1            ; memory block pointer in AX
            ldx lptr1+1
            jsr _freebstr
            lda lptr2           ; memory block pointer in AX
            ldx lptr2+1
            jsr _freebstr
            lda API_ERROR       ; How did it go?
            beq exit_ok         ; zero means success.
; Error exit when we still have one or two memory blocks to free:
            lda lptr1           ; return any mallocs we had
            ldx lptr1+1
            jsr _freebstr       ; no harm of they are null
            lda lptr2
            ldx lptr2+1
            jsr _freebstr       ; no harm of they are null
            lda #ENOENT         ; no such file or directory
            ldx #$0             
            rts

; WE are done, return filename buffer and exit:
exit_ok:    lda #$00    ; 0 = success return
            ldx #$00
            rts

.endproc
