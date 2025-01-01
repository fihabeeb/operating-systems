%include "asm_io.inc"


segment .data
    msg1    db          "Enter a number: ", 0
    msg2    db          "The sum of ", 0
    msg3    db          " and ", 0
    msg4    db          " is: ", 0

segment .bss
    integer1    resd  1 ;first int
    integer2    resd  1  ;second int
    result      resd 1  ;result

segment .text
        global asm_main
    asm_main:
        pusha


        mov eax, msg1           ; Eax points to msg1
        call print_string       ; Prints memory location of msg1
        call read_int           ; Inputs an int
        mov [integer1], eax     ; Stores input (which is in eax) in integer1
        mov eax, msg1           ; eax is now a pointer to msg1 again
        call print_string       ; Prints memory location of msg1
        call read_int           ; Inputs an int
        mov [integer2], eax     ; Stores input (which is in eax) in integer2
        mov eax, [integer1]     ; eax = first integer
        add eax, [integer2]     ; eax += second integer
        mov [result], eax       ; Stores result (which is in eax) in result
        mov eax, msg2           ; Eax points to msg2
        call print_string       ; Prints memory location of msg2
        mov eax, [integer1]     ; Value of integer1 is placed in eax
        call print_int          ; eax is printed
        mov eax, msg3           ; eax points to msg3
        call print_string       ; Prints memory location of msg3
        mov eax, [integer2]     ; Value of integer2 is placed in eax
        call print_int          ; eax is printed
        mov eax, msg4           ; eax points to msg3
        call print_string       ; Prints memory location of msg4
        mov eax, [result]       ; Value of result is placed in eax
        call print_int          ; eax is printed
        call print_nl           ; a new line is printed
        popa
        mov eax, 0              ; eax = 0
        ret
