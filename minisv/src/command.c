#include "command.h"
#include <string.h>
#include <stdio.h>
static int ParseError(char *error,
                      const char *message) {
    if (error != NULL) {
        snprintf(error, COMMAND_ERROR_MAX, "%s", message);
    }
    return 0;
}
static int NameIsValid(const char *name) {
    size_t len = strlen(name);
    if (len == 0 || len > TASK_NAME_MAX) return 0;

    for (size_t i = 0; i < len; ++i) {
        char c = name[i];
        if (!((c >= 'a' && c <= 'z') ||
              (c >= 'A' && c <= 'Z') ||
              (c >= '0' && c <= '9') ||
              c == '_' || c == '-')) {
            return 0;
        }
    }
    return 1;
}
int CommandParse(const char *line, Command_t *out, 
                    char *error) {

    if (line == NULL || out == NULL) {
        return ParseError(error, "解析函数参数为空");
    }

    memset(out, 0, sizeof(*out));
    out->type = CMD_EMPTY;

    size_t len = strlen(line);
    if (len > COMMAND_LINE_MAX) {
        return ParseError(error, "命令过长");
    }

    if (strpbrk(line, "\"'\\|<>;&$`*?[]") != NULL) {
        return ParseError(error, "暂不支持引号、转义、管道、重定向或通配符");
    }

    memcpy(out->storage, line, len + 1);

    // start + NAME + 参数
    char *tokens[PROGRAM_ARG_MAX + 2];
    int token_count = 0;

    char *token = strtok(out->storage, " \t\r\n\v\f");

    while (token != NULL) {
        if (token_count >= PROGRAM_ARG_MAX + 2) {
            return ParseError(error, "命令参数过多");
        }
        tokens[token_count++] = token;
        token = strtok(NULL, " \t\r\n\v\f");
    }

    if (token_count == 0) {
        return 0;
    }

    CommandType_t type = CMD_EMPTY;
    if (strcmp(tokens[0], "help") == 0) {
        type = CMD_HELP;
    } else if (strcmp(tokens[0], "start") == 0) {
        type = CMD_START;
    } else if (strcmp(tokens[0], "list") == 0) {
        type = CMD_LIST;
    } else if (strcmp(tokens[0], "stop") == 0) {
        type = CMD_STOP;
    } else if (strcmp(tokens[0], "stats") == 0) {
        type = CMD_STATS;
    } else if (strcmp(tokens[0], "quit") == 0) {
        type = CMD_QUIT;
    } else {
        return ParseError(error, "未知命令");
    }

    if (type == CMD_START) {
        if (token_count < 3) {
            return ParseError(error,
                              "用法：start NAME PROGRAM [ARGS...]");
        }

        if (!NameIsValid(tokens[1])) {
            return ParseError(error,
                              "名称须为 1～31 字节，仅允许字母、数字、_、-");
        }
        
        memcpy(out->name, tokens[1], strlen(tokens[1]) + 1);

        out->argc = token_count - 2;
        for (int i = 0; i < out->argc; i++) {
            out->argv[i] = tokens[i + 2];
        }
        out->argv[out->argc] = NULL;
    } else if (type == CMD_STOP) {
        if (token_count != 2) {
            return ParseError(error, "用法：stop NAME");
        }

        if (!NameIsValid(tokens[1])) {
            return ParseError(error,
                              "名称须为 1～31 字节，仅允许字母、数字、_、-");
        }
        
        memcpy(out->name, tokens[1], strlen(tokens[1]) + 1);
    } else {
        if (token_count != 1) {
            return ParseError(error, "命令不接受参数");
        }
    }
    out->type = type;
    return 0;
}