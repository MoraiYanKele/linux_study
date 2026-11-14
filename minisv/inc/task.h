#pragma once

#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define TASK_NAME_MAX 31
#define TASK_NUM_MAX 10

typedef enum {
    TASK_EMPTY,
    TASK_RUNNING,
    TASK_EXITED
} TaskState_t;

typedef struct {
    char name[32];
    pid_t pid;
    TaskState_t state;
    int wait_status;    /* 仅在 TASK_EXITED 时有效 */
} Task_t;

int TaskStart(Task_t tasks[], const char *name, char *argv[]);
int TaskReap(Task_t tasks[]);
void TaskList(const Task_t tasks[]);