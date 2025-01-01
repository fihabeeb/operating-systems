#include "drivers/interrupts.h"
#include "drivers/driver.h"
int __attribute__((cdecl)) sum_of_three(int arg1, int arg2, int arg3);
int __attribute__((cdecl)) multiply(int arg1, int arg2);
int __attribute__((cdecl)) subtract(int arg1, int arg2);


int sum_of_three(int arg1, int arg2, int arg3)
{
    return arg1 + arg2 + arg3;
}

int multiply(int arg1, int arg2)
{
    return arg1 * arg2;
}


int subtract(int arg1, int arg2)
{
    return arg1 - arg2;
}
int kmain()
{
    interrupts_install_idt();
    fb_clear();
    main_prompt[0] = ' ';
    main_prompt[1] = 'm';
    main_prompt[2] = 'i';
    main_prompt[3] = 'n';
    main_prompt[4] = 'i';
    main_prompt[5] = 'o';
    main_prompt[6] = 's';
    main_prompt[7] = '>';
    write_main_prompt();

    //fb_write_int(sum_of_three(1,2,3));
    //write_char(' ');
    //fb_write_int(multiply(10,5));
    //write_char(' ');
    //fb_write_int(subtract(50,20));


    while(1) {

    }

    return 0;
}
