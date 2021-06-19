#ifndef LAB1_COMMANDS_H
#define LAB1_COMMANDS_H
#include "string.h"
#include "limits.h"
struct commands parse_command(char* string);
enum com_type {
    LS = 0,
    PWD = 1,
    CD = 2,
    CP = 3,
    QUIT = 4,
    HELP = 5
};
struct commands {
    enum com_type type;
    char* first_argument;
    char* second_argument;
};
#endif
