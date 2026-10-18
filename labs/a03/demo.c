#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    puts("INFO: task started"); // 写入 stdout，并自动换行。
    fprintf(stderr, "ERROR: example message"); // 写入 stderr。

    /* 给错误信息补一个换行 */
    fputc('\n', stderr);

    if (argc > 1 && strcmp(argv[1], "fail") == 0) {
        return 7;
    }

    return true;
}