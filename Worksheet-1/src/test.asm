%include "asm_io.inc"


segment .data
    arr      times 100 dw 0; array of 100 0s
    index    dd  0 ;first int

segment .bss

segment .text
        global asm_main
    asm_main:
        pusha

        mov [arr], 15
        mov eax, [arr]      ;eax is sumthn
        call print_int
        call print_nl

    popa
    mov eax, 0              ; eax = 0
    ret
