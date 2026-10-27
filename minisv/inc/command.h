#ifndef COMMAND_H
#define COMMAND_H

#include <stddef.h>

#define COMMAND_LINE_MAX 1023
#define TASK_NAME_MAX 31
#define PROGRAM_ARG_MAX 16  // 传入参数最大值
#define COMMAND_ERROR_MAX 128 // 错误信息最大值

typedef enum {
    CMD_EMPTY,  // 
    CMD_HELP,   // 
    CMD_START,  // 
    CMD_LIST,   //
    CMD_STOP,   //
    CMD_STATS,  //
    CMD_QUIT    //
} CommandType_t;

typedef struct {
    CommandType_t type;
    char name[TASK_NAME_MAX + 1];
    char storage[COMMAND_LINE_MAX + 1];
    char *argv[PROGRAM_ARG_MAX + 1];
    int argc;

} Command_t;

int CommandParse(const char *line, Command_t *out, 
                    char *error);

#endif