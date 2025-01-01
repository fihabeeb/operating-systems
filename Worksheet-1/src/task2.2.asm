%include "asm_io.inc"


segment .data
    arr      times 100 dd 0; array of 100 0s
    index    dd  0 ;first int

segment .bss

segment .text
        global asm_main
    asm_main:
        pusha


        mov eax, 0      ;eax is sumthn
        mov ecx, 100    ; ecx is counter that goes down in the for loop
    loop_start:
        add eax, 1
        call print_int
        mov [arr + (eax * 4 - 4)], eax     ;index array here and give it value of eax
        loop loop_start
        mov eax, [arr + 4]
        call print_nl
        call print_int
        call print_nl
        mov eax, 0
        mov ecx, 100
    loop_2:
        add eax, [arr + (ecx * 4)]
        loop loop_2
        add eax,[arr]
    call print_int
    call print_nl

    popa
    mov eax, 0              ; eax = 0
    ret
