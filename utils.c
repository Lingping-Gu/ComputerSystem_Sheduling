#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "job.h"

static unsigned int seed;

void os_srand(unsigned int s) {
    seed = s;
}

int os_rand() {
    seed = (1103515245 * seed + 12345) % (1 << 31);
    return seed;
}

int IO_request() {
    return os_rand() % 10 == 0;  // CHANCE_OF_IO_REQUEST = 10
}

int IO_complete() {
    return os_rand() % 4 == 0;   // CHANCE_OF_IO_COMPLETE = 4
}

void print_stats(Job jobs[], int count, int total_time) {
    int total_completion = 0, min = 1 << 30, max = 0;
    int total_ready = 0, total_io = 0;

    printf("\nJob# | Total time in ready to run state | Total time in sleeping on I/O state | Total time in system |\n");
    printf("=====+==================================+=====================================+=======================+\n");

    for (int i = 0; i < count; i++) {
        int turnaround = jobs[i].end_time - jobs[i].arrival_time;
        printf("%-4d | %-32d | %-35d | %-22d |\n",
               jobs[i].pid,
               jobs[i].time_in_state[READY],
               jobs[i].time_in_state[WAITING],
               turnaround);

        total_completion += turnaround;
        if (turnaround < min) min = turnaround;
        if (turnaround > max) max = turnaround;

        total_ready += jobs[i].time_in_state[READY];
        total_io += jobs[i].time_in_state[WAITING];
    }

    printf("=====+==================================+=====================================+=======================+\n");
    printf("\nTotal simulation time: %d\n", total_time);
    printf("Total number of job: %d\n", count);
    printf("Shortest job completion time: %d\n", min);
    printf("Longest job completion time: %d\n", max);
    printf("Average Completion: %.0f\n", (float) total_completion / count);
    printf("Average Ready/Waiting: %.0f\n", (float)(total_ready + total_io) / count);
}
