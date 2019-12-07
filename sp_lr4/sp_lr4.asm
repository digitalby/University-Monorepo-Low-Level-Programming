; Turbo Assembler 5
; tasm sp_lr4.asm
; tlink sp_lr4.obj /t/x
; sp_lr4

    ideal
    model tiny
     
    codeseg
            org     100h
        start:
            jmp     install
    hook:
                push bx
                xor bx,bx
                
            cmp     ah, 4fh         
            jne     jmp_old

            cmp     al,1eh
            je jmp_ok_a
            cmp     al,12h
            je jmp_ok_e
            cmp     al,17h
            je jmp_ok_i
            cmp     al,18h
            je jmp_ok_o
            cmp     al,16h
            je jmp_ok_u
            
            jmp jmp_old
            jmp_ok_a:
            ; A was pressed
            mov al,12h
            jmp jmp_old
            jmp_ok_e:
            ; E was pressed
            mov al,17h
            jmp jmp_old
            jmp_ok_i:
            ; I was pressed
            mov al,18h
            jmp jmp_old
            jmp_ok_o:
            ; O was pressed
            mov al,16h
            jmp jmp_old
            jmp_ok_u:
            ; U was pressed
            mov al,1eh
            jmp jmp_old
        jmp_old:
        
            pop bx
            stc                     
        jmp_finret:

            db      0eah            
    to_ofs  dw      ?
    to_seg  dw      ?

     
     
        install:
            mov     ax, 3515h       
            int     21h
            mov     [to_seg], es
            mov     [to_ofs], bx
            push    cs
            pop     ds
            mov     dx, offset hook
            mov     ax, 2515h
            int     21h             

                        mov dx, offset install
            int 27h

DATASEG

scancode db 0

    end start
