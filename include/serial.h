#ifndef SERIAL_CONNECTION

#define SERIAL_CONNECTION

#define BAUDRATE 115200
#define MAX_LINE_SIZE 255

// Error codes
#define INVALID_COMMAND_ERROR 0

void serial_init();
void show_error(int code);

#endif