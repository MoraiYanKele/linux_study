#include <stdio.h>

#include "worker_output.h"

void worker_output(int count) {
    for (int i = 0; i < count; i++) {
        printf("seq=%d alive\n", i + 1);
        fflush(stdout);
    }
    fprintf(stderr, "demo_worker: test message\n");
}