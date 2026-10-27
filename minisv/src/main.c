#include "command.h"
#include <stdio.h>


int main () {
    char line[] = "start alpha ./demo_worker --count 5";
    Command_t command;
    char error[COMMAND_ERROR_MAX];
    if (CommandParse(line, &command, error) != 0) {
        fprintf(stderr, "%s\n", error);
    } else if (command.type == CMD_START) {
        printf("parsed: START name=%s program=%s argc=%d\n",
            command.name, command.argv[0], command.argc);

        for (int i = 0; i < command.argc; i++) {
            printf("argv[%d] = %s\n", i, command.argv[i]);
        }
    }   
    return 0;
}