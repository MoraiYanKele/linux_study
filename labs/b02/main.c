#include "log.h"

#include <stdio.h>
#include <string.h>

int main () {
    int fd = log_open("not_exist/app.log");

    if (fd == -1) {
        perror("log_open");
        return 1;
    }

    printf("fd = %d\n", fd);

    const char *message = "";
    int result = 0;
    char data[10000];
    memset(data, 'A', sizeof(data));
    log_write(fd, data, sizeof(data));

    if (log_close(fd) == -1) {
        perror("log_close");
        result = 1;
    }
    return result;
}
