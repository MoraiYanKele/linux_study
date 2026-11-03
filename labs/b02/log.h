#ifndef LOG_H
#define LOG_H

#include <stddef.h>

int log_open(const char *path);
int log_write(int fd, const void *data, size_t length);
int log_close(int fd);

#endif