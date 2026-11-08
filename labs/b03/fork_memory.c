#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main () {
    int value = 10;
    printf("before: pid =%ld value=%d, address=%p\n", (long)getpid(), value, (void*)&value);
    // fflush(stdout);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        value = 200;
        printf("child: pid =%ld ppid =%ld value=%d, address=%p, pid = %ld\n", (long)getpid(), (long)getppid(), value, (void*)&value, (long)pid);
        fflush(stdout);
    } else {
        value = 100;
        waitpid(pid, NULL, 0);
        printf("parent: pid =%ld ppid =%ld value=%d, address=%p， pid=%ld\n", (long)getpid(), (long)getppid(), value, (void*)&value, (long)pid);
        fflush(stdout);
    }
    return 0;

}