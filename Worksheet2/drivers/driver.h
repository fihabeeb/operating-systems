void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_move_cursor(unsigned short pos);
void write(char *buf, unsigned int len);
void fb_clear();
void move_cursor(unsigned int x, unsigned int y);
void fb_write_int(int num);
void set_text_color(unsigned char fg, unsigned char bg);
void write_char(char buf);
void backspace();
void new_line();
char getc();
void read_line();
void writeBuffer();
void clear_buffer();
void write_main_prompt();
void parse_buffer();
void create_commands();
void echo (char* args, unsigned int a);
void clear();
void nl_for_output();
void help();
void display_version();
void shutdown();
void colour(char* args, unsigned int length);
#define HELP_MESSAGE "Clear - Clears the screen\nEcho - Copies the tests after it\nVersion - Prints system version\nShutdown - Prepares the system for shutdown"
#define COMMAND_NOT_FOUND   "Unknown Command, check spelling and try again."
#define BUFFER_SIZE 256
char my_buffer[BUFFER_SIZE];
char main_prompt[7];
#define FB_FOREGROUND 2
#define FB_BACKGROUND 8
