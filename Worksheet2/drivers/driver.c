#include "io.h"
#include "driver.h"
/** fb_write_cell:
* Writes a character with the given foreground and background to position i
* in the framebuffer.
* @param i The location in the framebuffer
* @param c The character
* @param fg The foreground color
* @param bg The background color
*/

static unsigned int cursor_pos = 0;

unsigned int written_chars = 0;

unsigned int buffer_chars = 0;

char *fb = (char *) 0x000B8000;


unsigned int fb_foreground = 0;
unsigned int fb_background = 15;


struct command {
    char* name;
    void (*function)(char* args, unsigned int a);
};

struct command commands[] =
    {
        {"help",help},
        {"echo", echo},
        {"clear",clear},
        {"shutdown",shutdown},
        {"version", display_version},
        {"colour", colour}
    };

unsigned int num_commands = 6;

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}




/* The I/O ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

/** fb_move_cursor:
* Moves the cursor of the framebuffer to the given position
*
* @param pos The new position of the cursor
*/

void fb_move_cursor(unsigned short pos)
{
    pos /= 2;
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);

    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));

    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);

    outb(FB_DATA_PORT, pos & 0x00FF);
}


void write(char *buf, unsigned int len) {

    for (unsigned int i = 0; i < len; i++) {
        char c = buf[i];

        if (c == '\0') {
            break;
        }
        else if (c == '\n')
        {
            cursor_pos /= 160;
            cursor_pos *= 160;
            cursor_pos += 160;
            cursor_pos += 14;
            write_char('\0');
            fb_move_cursor(cursor_pos);
        }
        else {
            fb_write_cell(cursor_pos, c, fb_foreground, fb_background);
            cursor_pos += 2;
            written_chars++;
        }

    }
    //cursor_pos = 80;
    //79 rows per column? 80 goes to next row
    //cursor_pos = 160;
    //yeah this goes to row 3. Found throgh trial and error, every 80 leads to a new line
    fb_move_cursor(cursor_pos);
}







void write_char(char buf) {

    fb_write_cell(cursor_pos, buf, fb_foreground,fb_background);

    cursor_pos += 2;
    written_chars++;
    fb_move_cursor(cursor_pos);
}



// Clear the framebuffer
void fb_clear() {
    unsigned int mode = 0;
    for (unsigned int i = 0; i < 80 * 25; i++) {
        if (mode == 0)
        {
            fb[i] = ' ';
            mode = 1;
        }
        else if (mode == 1)
        {
            fb[i] = ((0x00 & 0x0F) << 4) | (0x0F & 0x0F); // Clear with spaces PUT WHITE ON BLACK (THATS WHY)
            mode = 0;
        }
    }
    written_chars = 0;
    fb_move_cursor(0);
}



// Move the cursor to a specific (x, y) location
void move_cursor(unsigned int x, unsigned int y) {
    if (x < 80 && y < 25) {
        fb_move_cursor(y * 80 + x);
    }
}


void fb_write_int(int num) {
    char buffer[12];
    char *ptr = &buffer[11];
    *ptr = '\0';

    if (num == 0) {
        fb_write_cell(cursor_pos,'0', fb_foreground, fb_background);
        fb_move_cursor(cursor_pos);
        return;
    }

    int is_negative = 0;
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    while (num > 0) {
        *(--ptr) = (num % 10) + '0';
        num /= 10;
    }

    if (is_negative) {
        *(--ptr) = '-';
    }


    write(ptr,sizeof(buffer));
    fb_move_cursor(cursor_pos);
}

void set_text_color(unsigned char fg, unsigned char bg) {
    for (unsigned int i = 1; i < cursor_pos + 160; i += 2) {
        fb[i] = ((fg & 0x0F) << 4) | (bg & 0x0F);
    }
    fb_background = bg;
    fb_foreground = fg;
}





void backspace()
{
    //static unsigned int limiter = 0;
    //limiter = cursor_pos / 160;
    //limiter = cursor_pos * 160;

    if (written_chars > 0)
    {
        fb_write_cell(cursor_pos - 2, ' ', fb_foreground, fb_background);
        cursor_pos -= 2;
        fb_move_cursor(cursor_pos);
        if (written_chars > 0)
        {
            written_chars--;
        }
    }
}

char getc()
{
    char retval = fb[cursor_pos - 2];
    //fb[cursor_pos - 2] = ' ';
    return retval;
}

void read_line()
{/*
    char holder;
    unsigned int count = 0;
    static unsigned int limiter = 0;
    limiter = cursor_pos / 160;
    limiter = cursor_pos * 160;
    while (holder != '\n' && cursor_pos > limiter)
    {
        for (int w = count; w > 0; w--)
        {
            my_buffer[w] = my_buffer[w - 1];
        }
        holder = getc();
        my_buffer[0] = holder;
        cursor_pos -= 2;
        count++;
    }
    fb_move_cursor(cursor_pos);
    buffer_chars = written_chars;
    written_chars = 0;
    */

    char holder;
    unsigned int count = 0;
    static unsigned int limiter = 0;
    limiter = cursor_pos / 160;
    limiter = cursor_pos * 160;

    while (1) {
        holder = getc();
        if (holder == '\n' || count >= BUFFER_SIZE - 1 || cursor_pos >= limiter || holder == '\0') {
            break;
        }

        for (int w = count; w > 0; w--)
        {
            my_buffer[w] = my_buffer[w - 1];
        }

        my_buffer[0] = holder;
        count++;
        cursor_pos -= 2;//backspace(); // Clear framebuffer as we read backward
    }
    my_buffer[count] = '\0';
    buffer_chars = count;
    written_chars = 0;
}


void new_line()
{
    clear_buffer();
    read_line();
    parse_buffer();
    //writeBuffer();
    //written_chars = 0;
    cursor_pos /= 160;
    cursor_pos *= 160;
    //cursor_pos = (cursor_pos / 160 + 1) * 160;
    /*
    while (cursor_pos % 80 != 0)
    {
        cursor_pos -= 1;
    }
    */
    cursor_pos += 160;
    //writeBuffer();
    write_main_prompt();
}

void writeBuffer() {
    if (buffer_chars > 0) {
        write(my_buffer, buffer_chars);
        clear_buffer();
    }
}

void clear_buffer()
{
    for (unsigned int l = 0; l < sizeof(my_buffer) + 1; l++)
    {
        my_buffer[l] = ' ';
    }
    buffer_chars = 0;
}


void write_main_prompt()
{
    for (unsigned int i = 1; i < 8; i++) {
        char c = main_prompt[i];
        fb_write_cell(cursor_pos, c, fb_foreground, fb_background);
        cursor_pos += 2;
    }
    write_char('\0');
    written_chars--;
    fb_move_cursor(cursor_pos);
}


void parse_buffer()
{
    char command_name[buffer_chars];
    char params[buffer_chars];
    int mode = 0;
    int counter = 0;
    unsigned int len_command = 0;
    unsigned int len_params = 0;
    int spaced = 0;
    int finished = 0;
    for (unsigned int x = 0; x < buffer_chars; x++)
    {
        if (my_buffer[x] == '\0')
        {
            break;
        }
        else if (my_buffer[x] == ' ' && spaced == 0)
        {
            spaced = 1;
            mode = 1;
            continue;
        }
        switch (mode)
        {
            case 0:
            command_name[x] = my_buffer[x];
            len_command++;
            break;
            case 1:
            params[counter] = my_buffer[x];
            len_params++;
            counter++;
            break;
        }

    }


    for (unsigned int q = 0; q < num_commands; q++)
    {
        int found = 0;
        for (unsigned int y = 0; y < len_command; y++)
        {
            if (command_name[y] != commands[q].name[y])
            {
                found = 1;
                break;
            }
        }

        if (found == 1)
        {
            continue;
        }
        commands[q].function(params,len_params);
        finished = 1;
        break;
    }
    if (finished == 0)
    {
        nl_for_output();
        write(COMMAND_NOT_FOUND, sizeof(COMMAND_NOT_FOUND));
    }
}

void echo (char* args, unsigned int a)
{
    nl_for_output();
    write(args, a);
}

void clear()
{
    fb_clear();
    cursor_pos = 0;
}

void help()
{
    nl_for_output();
    write(HELP_MESSAGE,sizeof(HELP_MESSAGE));
}

void display_version()
{
    nl_for_output();
    write("Mini OS ver 1.0.0.1",sizeof("Mini OS ver 1.0.0.1"));
}

void shutdown()
{
    nl_for_output();
    write("System ready for shutdown",sizeof("System ready for shutdown"));
}

void nl_for_output()
{
    cursor_pos /= 160;
    cursor_pos *= 160;
    cursor_pos += 160;
    for (unsigned int i = 1; i < 8; i++) {
        char c = main_prompt[i];
        fb_write_cell(cursor_pos, c, fb_foreground,fb_background);
        cursor_pos += 2;
    }
    write_char('\0');
    fb_move_cursor(cursor_pos);
}

struct colour {
    char* name;
    unsigned int id;
};

struct colour all_colours[] = {{"black", 0}, {"white", 15}, {"blue",1},{"green",2},{"cyan",3},{"dark-grey",8},{"light-blue",9},{"light-green",10},
                                {"light-cyan",11},{"light-red",12},{"light-magenta",13},{"light-brown",14},{"red",4},{"magenta",5},{"brown",6},{"light-grey",7}};

char* colours;
char* colour2;

void colour(char* args, unsigned int length)
{
    int mode = 1;
    unsigned int index = 0;
    unsigned int coloura = 0;
    unsigned int colourb = 0;
    int founda = 0;
    int foundb = 0;
    for (unsigned int x = 0; x < length; x++)
    {
        if (args[x] == '\0')
        {
            break;
        }
        else if (args[x] == ' ')
        {
            if (mode == 1)
            {
                mode = 2;
                continue;
            }
            else if (mode == 2)
            {
                break;
            }
        }
        else
        {
            if (mode == 1)
            {
                colours[x] = args[x];
            }
            else if (mode == 2)
            {
                colour2[x] = args[index];
                index++;
            }
        }
    }
    for (unsigned int x = 0; x < 16; x++)
    {
        for (int l = 0; l < 5; l++)
        {
            if (colours[l] != all_colours[x].name[l])
            {
                break;
            }
            coloura = all_colours[x].id;
            founda = 1;
        }
        if (founda == 1)
        {
            break;
        }
    }
    for (unsigned int w = 0; w < 16; w++)
    {
        for (int l = 0; l < 5; l++)
        {
            if (colour2[l] != all_colours[w].name[l])
            {
                break;
            }
            colourb = all_colours[w].id;
            foundb = 1;
        }
        if (foundb == 1)
        {
            break;
        }
    }

    if (founda == 1 && foundb == 1)
    {
        set_text_color(coloura,colourb);
    }
}