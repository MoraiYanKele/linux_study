#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

int TaskStart(Task_t tasks[], const char *name, char *argv[]) {
    for (int i = 0; i < 2; i++) {
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
                execv(argv[0], argv);
                perror("execv");
                _exit(127);
            }
            strcpy(tasks[i].name, name);    
            tasks[i].pid = pid;
            tasks[i].state = TASK_RUNNING;
            strcpy(tasks[i].name, name);
            break;
        }
    }
    return 0;
}

int TaskReap(Task_t tasks[]) {
    while (1) {
        int status;
        pid_t wait_pid = waitpid(-1, &status, WNOHANG);
        if (wait_pid == 0) {
            break;
        } 
        if (wait_pid > 0) {
            for (int i = 0; i < 2; i++) {
                if (tasks[i].state == TASK_RUNNING && tasks[i].pid == wait_pid) {
                    tasks[i].state = TASK_EXITED;
                    tasks[i].wait_status = status;
                    printf("pid: %d exited, status=%d\n", wait_pid, WEXITSTATUS(status));
                }
            }
        }
        if (wait_pid == -1) {
            if (errno == ECHILD) {
                break;
            }
        }
    }
    return 0;
}

void TaskList(const Task_t task[]) {
    for (int i = 0; i < 2; i++) {
        printf("%d, name: %s, pid=%d, state=%d\n", i, task[i].name, task[i].pid, task[i].state);
    }
}



int main(int argc, char *argv[]) {
    Task_t tasks[2];
    tasks[0] = (Task_t){
        .name = "",
        .pid = 0,
        .state = TASK_EMPTY,
        .wait_status = 0
    };
    tasks[1] = (Task_t){
        .name = "",
        .pid = 0,
        .state = TASK_EMPTY,
        .wait_status = 0
    };

    char *worker1_argv[] = {"./labs/a05/demo_worker", 
                            "--count", "5",
                            "--exit-code", "0",
                            "--interval-ms", "2000", NULL};
    char *worker2_argv[] = {"./labs/a05/demo_worker",
                            "--count", "2",
                            "--exit-code", "1",
                            "--interval-ms", "100", NULL};

    TaskStart(tasks, "worker1", worker1_argv);
    TaskStart(tasks, "worker2", worker2_argv);

    while (1) {
        TaskReap(tasks);
        TaskList(tasks);
        sleep(1);
    }   

    return 0;
}