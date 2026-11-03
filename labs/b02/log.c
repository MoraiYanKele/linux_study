#include "log.h"

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int log_open(const char *path) {
    return open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
}
int log_write(int fd, const void *data, size_t length) {
    const char *p = data;

    while (length > 0) {
        ssize_t n = write(fd, p, length);
        if (n < 0) {
            if (errno ==EINTR) {
                continue;
            }
            return -1;
        }
        if (n == 0) {
            errno = EIO;
            return -1;
        }
        p += n;
        length -= (size_t)n;
    }
    return 0;
}

int log_close(int fd) {
    return close(fd);
}