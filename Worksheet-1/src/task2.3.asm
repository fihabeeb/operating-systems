%include "asm_io.inc"


segment .data
    arr      times 100 dd 0; array of 100 0s
    index    dd  0 ;first int
    msg1    db          "Enter first num of range: ", 0
    msg2    db          "Enter second num of range: ", 0
    msg3  db  'range too small' ,0;
    msg4   db   'range too big', 0;

segment .bss
    rangeA  resd  1  ;int
    rangeB  resd  1  ;int
segment .text
        global asm_main
    asm_main:
        pusha


    reinput:
        mov eax, msg1
        call print_string
        call read_int
        mov [rangeA], eax

        mov eax, msg2
        call print_string
        call read_int
        mov [rangeB], eax

        mov ebx, [rangeA]
        mov esi, [rangeB]
        mov ecx, 99
    loop_2:
        add ebx, 1
        loop loop_2
        cmp ebx, esi
        JE validrange
        JL rangebig
        JG rangesmall

    rangebig:
        mov eax, msg4
        call print_string
        call print_nl
        jmp reinput
    rangesmall:
        mov eax, msg3
        call print_string
        call print_nl
        jmp reinput

validrange:
        mov eax, [rangeA]      ;eax is sumthn
        mov ecx, 100    ; ecx is counter that goes down in the for loop
        mov esi, 0
    loop_start:
        add esi, 1
        mov [arr + (esi * 4 - 4)], eax     ;index array here and give it value of eax
        add eax, 1
        loop loop_start
        mov eax, [arr + 4]
        mov eax, 0
        mov ecx, 100
    loop_3:
        add eax, [arr + (ecx * 4)]
        loop loop_3
        add eax,[arr]
    call print_int
    call print_nl

    popa
    mov eax, 0              ; eax = 0
    ret
