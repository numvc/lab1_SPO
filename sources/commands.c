#include "../headers/commands.h"

enum com_type get_type(char* string){
    if (strcmp(string, "ls") == 0) {
        return LS;
    }else if (strcmp(string, "pwd") == 0){
        return PWD;
    }else if (strcmp(string, "cd") == 0){
        return CD;
    }else if (strcmp(string, "cp") == 0){
        return CP;
    }else if (strcmp(string, "quit") == 0){
        return QUIT;
    }else return HELP;
}

struct commands parse_command(char* string) {
    struct commands command;
    char* string_mem = strdup(string);
    string_mem = strtok(string_mem, " ");
    int i = 0;
    while (string_mem != NULL){
        if (i == 0) {
            command.type = get_type(string_mem);
        }else if (i == 1) {
            command.first_argument = string_mem;
        }else if (i == 2) {
            command.second_argument = string_mem;
        }
        i ++;
        string_mem = strtok(NULL, " ");
    }
    return command;
}

