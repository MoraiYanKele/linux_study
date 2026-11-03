#include <stdio.h>
#include <time.h>
#include "worker_output.h"


void worker_output(int count, int interval_ms) {
    
    struct timespec ts = {
        .tv_sec = interval_ms / 1000,
        .tv_nsec = (interval_ms % 1000) * 1000000L
    };

    for (int i = 0; i < count; i++) {
        printf("seq=%d alive\n", i + 1);
        fflush(stdout);

        if (i + 1 < count) {
            nanosleep(&ts, NULL);
        }
    }
    fprintf(stderr, "demo_worker: test message\n");
}