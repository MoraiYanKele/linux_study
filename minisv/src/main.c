#include "command.h"
#include <stdio.h>
#include <string.h>


int main () {
    char line[COMMAND_LINE_MAX + 2];
    Command_t command;
    char error[COMMAND_ERROR_MAX];

    while (1) {
        printf("minisv> ");
        fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            if (ferror(stdin)) {
                perror("fgets");
                return 1;
            }
            return 0;
        }
        if (CommandParse(line, &command, error) != 0) {
            fprintf(stderr, "%s\n", error);
            continue;
        }
        switch (command.type) {
            case CMD_EMPTY:
                break;
            case CMD_HELP:
                printf("parsed: HELP\n");
                break;
            case CMD_START:
                printf("parsed: START name=%s program=%s argc=%d\n",
                    command.name, command.argv[0], command.argc);

                for (int i = 0; i < command.argc; i++) {
                    printf("argv[%d] = %s\n", i, command.argv[i]);
                }
                break;
            case CMD_LIST:
                printf("parsed: LIST\n");
                break;
            case CMD_STOP:
                printf("parsed: STOP name=%s\n", command.name);
                break;
            case CMD_STATS:
                printf("parsed: STATS\n");
                break;
            case CMD_QUIT:
                printf("parsed: QUIT\n");
                return 0;
        }
    }
    return 0;
}