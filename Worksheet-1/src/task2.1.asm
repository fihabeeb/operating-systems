%include "asm_io.inc"


segment .data
    msg1    db          "Enter Name: ", 0
    msg2    db          "Print Count: ", 0
    errmessagesmall  db  'too small' ,0;
    errmessagebig   db   'too big', 0;
    index   dd 0;

segment .bss
    printcount  resd  1  ;int
    name1       resd  10 ;username

segment .text
        global asm_main
    asm_main:
        pusha

        mov eax, msg1
        call print_string
        mov ecx, 10
        mov ebx, 0
    loopfirst:
        call read_char;
        cmp eax, 10
        je endloop
        mov [name1 + (ebx * 4 + 4)],eax
        add ebx, 1
        loop loopfirst
    small:
        mov eax, errmessagesmall
        call print_string
        call print_nl
        jmp reinput
    big:
        mov eax, errmessagebig
        call print_string
        call print_nl
        jmp reinput
    endloop:
        add ebx, 1
        mov [index], ebx
        mov [name1], eax
    reinput:
        mov eax, msg2
        call print_string
        call read_int
        mov [printcount], eax
        mov eax, [name1]
        mov ecx, [printcount]
        ; Check if printcount is less than 100 and greater than 50
        cmp ecx,50
        JL small
        cmp ecx, 100
        JG big
    loopstart:
        mov ebx, ecx
        mov ecx, [index]
        mov edx, 0
        loopname:
            mov eax,[name1 + (edx * 4)]
            call print_char
            add edx, 1
            loop loopname
        mov ecx, ebx
        call print_nl
        loop loopstart


        popa
        mov eax, 0              ; eax = 0
        ret
