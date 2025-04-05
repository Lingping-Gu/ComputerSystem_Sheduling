#include <stdio.h>
#include <stdlib.h>
#include "job.h"
#include "queue.h"
#include "utils.h"
#include "schedulers.h"

#define MAX_JOBS 100

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <scheduling_policy>\n", argv[0]);
        fprintf(stderr, "Policies: R = Round Robin, S = Shortest Job First, M = MLFQ\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Error opening input file");
        return 1;
    }

    char policy = argv[2][0];
    Job jobs[MAX_JOBS];
    int job_count = 0;

    // Initialize deterministic RNG seed
    os_srand(1);

    // Read input file
    while (fscanf(fp, "%d:%d:%d:%d", &jobs[job_count].pid,
                                       &jobs[job_count].arrival_time,
                                       &jobs[job_count].service_time,
                                       &jobs[job_count].priority) != EOF) {
        jobs[job_count].remaining_time = jobs[job_count].service_time;
        jobs[job_count].state = NEW;
        init_job_stats(&jobs[job_count]);
        job_count++;
    }
    fclose(fp);

    // Select scheduling policy
    switch (policy) {
        case 'R':
            round_robin(jobs, job_count);
            break;
        case 'S':
            shortest_job_first(jobs, job_count);
            break;
        case 'M':
            multilevel_feedback_queue(jobs, job_count);
            break;
        default:
            fprintf(stderr, "Unknown scheduling policy '%c'\n", policy);
            return 1;
    }

    return 0;
}
