#include <stdio.h>
#include <time.h>
#include "worker_output.h"
#include <signal.h>
#include <time.h>

static volatile sig_atomic_t stopRequested = 0;
static void HandleTerm(int signo) {
    (void) signo;
    stopRequested = 1;
}

void worker_output(int count, int interval_ms, int ignore_trem) {
    
    stopRequested = 0;

    struct sigaction action = {0};
    sigemptyset(&action.sa_mask);

    if (ignore_trem) {
        action.sa_handler = SIG_IGN; // 忽略 SIGTERM 信号
    } else {
        action.sa_handler = HandleTerm; // 处理 SIGTERM 信号
    }

    if (sigaction(SIGTERM, &action, NULL) == -1) {
        perror("sigaction");
        return;
    }

    struct timespec ts = {
        .tv_sec = interval_ms / 1000,
        .tv_nsec = (interval_ms % 1000) * 1000000L
    };

    printf("worker ready\n");
    fflush(stdout);

    for (int i = 0; i < count; i++) {
        if (stopRequested) {
            break;
        }
        printf("seq=%d alive\n", i + 1);
        fflush(stdout);

        if (i + 1 < count) {
            nanosleep(&ts, NULL);
        }
    }
    if (stopRequested) {
        fprintf(stderr, "worker: received SIGTERM, exiting\n");
    } else {
        fprintf(stderr, "worker: finished\n");
    }
}