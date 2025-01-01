global loader               ; the entry symbol for ELF

    MAGIC_NUMBER    equ 0x1BADB002          ; define the magic number constant
    FLAGS           equ 0x0                 ; multiboot flags
    CHECKSUM        equ -MAGIC_NUMBER       ; calculate the checksum
                                            ; (magic number + checkksum + flags "should"
                                            ;    equal 0)

    KERNEL_STACK_SIZE equ 4096 ; size of stack in bytes

    section .data
        message db "LETS GOO!\n Working???"
        message_len equ $ - message            ; Length of the message
    section .bss
        align 4 ; align at 4 bytes
        kernel_stack: ; label points to beginning of memory
            resb KERNEL_STACK_SIZE ; reserve stack for the kernel

    section .text                            ; start of the text (code) section
        align 4                                 ; the code must be 4 byte aligned
            dd MAGIC_NUMBER                     ; write the magic number to the machine code,
            dd FLAGS                            ; the flags,
            dd CHECKSUM                         ; and the checksum
            extern sum_of_three                 ; extern not external?
            extern multiply                     ; extern not external?
            extern subtract                     ; extern not external?
            extern fb_write_cell
            extern fb_move_cursor
            extern write
            extern fb_clear
            extern move_cursor
            extern fb_write_int
            extern set_text_color
            extern enable_hardware_interrupts
            extern disable_hardware_interrupts
            extern kmain


    loader:                                 ; the loader label (defined as entry point in linker script)
        mov eax, 0xCAFEBABE                 ; place the number 0xCAFEBABE in the register eax
        mov esp, kernel_stack + KERNEL_STACK_SIZE   ; point esp to the start of the
                                                    ; stack (end of memory area)


        call kmain
        ;call enable_hardware_interrupts
        ;push dword 3 ; arg3
        ;push dword 2 ; arg2
        ;push dword 1 ; arg1
        ;call sum_of_three ; call the function, the result will be in eax

        ;push dword 10; arg 2
        ;push dword 1 ; arg1
        ;call multiply
        ;push dword 5; arg2
        ;push dword 10; arg1
        ;call subtract

        ;push byte 0x00                      ; Printed on a bg colour
        ;push byte 0x02
        ;push byte 'A'
        ;push dword 0
        ;call fb_write_cell


        ;push dword 5
        ;call fb_move_cursor    moves cursor to the position of dword starting from top leftbeing 0


        ;push message_len
        ;push message
        ;call write


        ;push dword 50185
        ;call fb_write_int

        ;push dword 2
        ;push dword 1
        ;call move_cursor
        ;call fb_clear

        ;push byte 0x09
        ;push byte 0x02
        ;call set_text_color
    .loop:
        jmp .loop                           ; loop foreva
