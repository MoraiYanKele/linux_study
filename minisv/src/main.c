#include "command.h"
#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int run_program(char *args[]) {
    fflush(NULL);
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return -1;
    }
    if (pid == 0) {
        execvp(args[0], args);
        perror("execvp");
        _exit(127);
    }
    int status;
    int res =  waitpid(pid, &status, 0);
    printf("res = %d\n",res);
    if (WIFEXITED(status)) {
        printf("child pid=%ld exited, code=%d\n",
               (long)pid, WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("child pid=%ld killed by signal=%d\n",
               (long)pid, WTERMSIG(status));
    }
    return 0;
}

int main (int argc, char *argv[]) {
    char line[COMMAND_LINE_MAX + 2];
    Command_t command;
    char error[COMMAND_ERROR_MAX];
    if (argc >= 3 && strcmp(argv[1], "--run") == 0) {
        return run_program(&argv[2]);
    }

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