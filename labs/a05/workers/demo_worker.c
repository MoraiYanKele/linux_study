#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "worker_output.h"

/* 将字符串转换为指定范围内的整数。
 * 成功返回 0，失败返回 -1。
 */
static int parse_int(const char *text, int min, int max, int *result)
{
    char *end;

    errno = 0;
    long value = strtol(text, &end, 10);

    /* 没有读到数字，或数字后面还有其他字符 */
    if (end == text || *end != '\0') {
        return -1;
    }

    /* 转换溢出，或超出允许范围 */
    if (errno == ERANGE || value < min || value > max) {
        return -1;
    }

    *result = (int)value;
    return 0;
}

int main(int argc, char *argv[])
{
    int count = 5;       /* 默认输出 5 条 */
    int exit_code = 0;   /* 默认返回 0 */

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--count") == 0) {
            /* 先检查后面是否有值，再访问 argv[i + 1] */
            if (i + 1 >= argc) {
                fprintf(stderr, "--count 缺少参数值\n");
                return 2;
            }

            i++; /* 跳到参数值 */
            if (parse_int(argv[i], 0, 100000, &count) != 0) {
                fprintf(stderr,
                        "--count 必须是 0～100000 的整数：%s\n",
                        argv[i]);
                return 2;
            }
        } else if (strcmp(argv[i], "--exit-code") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "--exit-code 缺少参数值\n");
                return 2;
            }

            i++;
            if (parse_int(argv[i], 0, 125, &exit_code) != 0) {
                fprintf(stderr,
                        "--exit-code 必须是 0～125 的整数：%s\n",
                        argv[i]);
                return 2;
            }
        } else {
            fprintf(stderr, "未知参数：%s\n", argv[i]);
            fprintf(stderr,
                    "用法：%s [--count N] [--exit-code C]\n",
                    argv[0]);
            return 2;
        }
    }

    worker_output(count);

    return exit_code;
}