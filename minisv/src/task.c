#include "task.h"

// 启动一个新任务
int TaskStart(Task_t tasks[], const char *name, char *argv[]) {
    if (name  == NULL || strlen(name) > TASK_NAME_MAX || strlen(name) == 0) {
        printf("任务名过长或为空: %s\n", name);        
        return -1;
    }
    for (int i = 0; i < TASK_NUM_MAX; i++) {
        if (tasks[i].state != TASK_EMPTY && strcmp(tasks[i].name, name) == 0) {
            printf("任务名已存在: %s\n", name);
            return -1;
        }
    }
    for (int i = 0; i < TASK_NUM_MAX; i++) {
        if (tasks[i].state == TASK_EMPTY) {

            if (strlen(tasks[i].name) > 0) {
                continue;
            }
            fflush(NULL);

            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
                return -1;
            }
            if (pid == 0) {
                execvp(argv[0], argv);
                perror("execvp");
                _exit(127);
            }
            strcpy(tasks[i].name, name);    
            tasks[i].pid = pid;
            tasks[i].state = TASK_RUNNING;
            strcpy(tasks[i].name, name);
            return 0;
        }
    }
    fprintf(stderr, "任务列表已满，无法启动新任务: %s\n", name);
    return -1;
}



// 回收已退出的子进程，并更新任务记录
int TaskReap(Task_t tasks[]) {
    while (1) {
        int status;
        pid_t wait_pid = waitpid(-1, &status, WNOHANG);
        if (wait_pid == 0) {
            break;
        } 
        if (wait_pid > 0) {
            for (int i = 0; i < TASK_NUM_MAX; i++) {
                if (tasks[i].state == TASK_RUNNING && tasks[i].pid == wait_pid) {
                    tasks[i].state = TASK_EXITED;
                    tasks[i].wait_status = status;
                    if (WIFEXITED(status)) {
                        printf("pid: %d exited, status=%d\n", wait_pid, WEXITSTATUS(status));
                    } else if (WIFSIGNALED(status)) {
                        printf("pid: %d killed by signal %d\n", wait_pid, WTERMSIG(status));
                    } else {
                        printf("pid: %d exited with unknown status\n", wait_pid);
                    }
                    
                }
            }
        }
        if (wait_pid == -1) {
            if (errno == EINTR) {
                continue;
            }
            if (errno == ECHILD) {
                break;
            }
            perror("waitpid");
            return -1;
        }
        
    }
    return 0;
}

// 列出所有任务
void TaskList(const Task_t task[]) {
    for (int i = 0; i < TASK_NUM_MAX; i++) {
        printf("%d, name: %s, pid=%d, state=%d\n", i, task[i].name, task[i].pid, task[i].state);
    }
}