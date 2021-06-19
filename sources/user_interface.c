#include "../headers/user_interface.h"
#include "../headers/operations_ext.h"
#include "limits.h"

void execute_operation(struct commands command, struct state* fs_state){
    char* result;
    switch (command.type) {
        case LS:
            printf("%s\n", execute_ls(fs_state));
            break;
        case PWD:
            printf("%s\n", execute_pwd(fs_state));
            break;
        case CD:
            result = execute_ext_cd(fs_state, command.first_argument);
            if (result != NULL) printf("%s\n", result);
            break;
        case CP:
            execute_ext_cp(fs_state, command.first_argument, command.second_argument);
            break;
        default:
            execute_help();
    }
}

int entrance(char* path){
    struct state* fs_state = setup(path);
    if (fs_state != NULL){
        printf("Введите команду\n");
        char input[LINE_MAX];
        fgets_wrapper(input, LINE_MAX, stdin);
        struct commands command = parse_command(input);
        while (command.type != QUIT){
            execute_operation(command, fs_state);
            printf("Введите команду\n");
            fgets_wrapper(input, LINE_MAX, stdin);
            command = parse_command(input);
        }
        printf("Завершение работы.\n");
        return 0;
    }
    return 1;
}
char *fgets_wrapper(char *buffer, size_t buflen, FILE *fp) {
    if (fgets(buffer, buflen, fp) != 0)
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        return buffer;
    }
    return 0;
}
