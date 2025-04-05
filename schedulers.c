// schedulers.c
#include <stdio.h>
#include "schedulers.h"
#include "queue.h"
#include "utils.h"
#include "job.h"

#define TIME_SLICE 4
#define MLFQ_LEVELS 5
const int time_slices[] = {1, 2, 4, 8, 16};
const int AGING_THRESHOLD = 20;

void round_robin(Job jobs[], int count) {
    Queue *ready = create_queue();
    Queue *io = create_queue();
    int completed = 0, clock = 0, slice = 0;
    Job *current = NULL;

    while (completed < count) {
        for (int i = 0; i < count; i++)
            if (jobs[i].arrival_time == clock && jobs[i].state == NEW)
                enqueue(ready, &jobs[i]), jobs[i].state = READY;

        int io_jobs = io->count;
        for (int i = 0; i < io_jobs; i++) {
            Job *j = dequeue(io);
            if (IO_complete()) enqueue(ready, j), j->state = READY;
            else enqueue(io, j);
        }

        if (!current && !is_empty(ready)) {
            current = dequeue(ready);
            current->state = RUNNING;
            slice = 0;
        }

        if (current) {
            current->remaining_time--;
            slice++;
            current->total_cpu_time++;

            if (IO_request()) {
                current->state = WAITING;
                enqueue(io, current);
                current = NULL;
                slice = 0;
            } else if (current->remaining_time == 0) {
                current->state = FINISHED;
                current->end_time = clock + 1;
                current = NULL;
                completed++;
            } else if (slice == TIME_SLICE) {
                current->state = READY;
                enqueue(ready, current);
                current = NULL;
            }
        }
        clock++;
    }

    printf("Scheduling Policy: Round Robin\n");
    print_stats(jobs, count, clock);
    free_queue(ready);
    free_queue(io);
}

void shortest_job_first(Job jobs[], int count) {
    Queue *io = create_queue();
    int completed = 0, clock = 0;
    Job *current = NULL;

    while (completed < count) {
        for (int i = 0; i < count; i++)
            if (jobs[i].arrival_time == clock && jobs[i].state == NEW)
                jobs[i].state = READY;

        int io_jobs = io->count;
        for (int i = 0; i < io_jobs; i++) {
            Job *j = dequeue(io);
            if (IO_complete()) j->state = READY;
            else enqueue(io, j);
        }

        Job *shortest = NULL;
        for (int i = 0; i < count; i++)
            if (jobs[i].state == READY)
                if (!shortest || jobs[i].remaining_time < shortest->remaining_time ||
                    (jobs[i].remaining_time == shortest->remaining_time && jobs[i].pid < shortest->pid))
                    shortest = &jobs[i];

        if (shortest && (current == NULL || shortest->remaining_time < current->remaining_time)) {
            if (current) current->state = READY;
            current = shortest;
            current->state = RUNNING;
        }

        if (current) {
            current->remaining_time--;
            current->total_cpu_time++;

            if (IO_request()) {
                enqueue(io, current);
                current->state = WAITING;
                current = NULL;
            } else if (current->remaining_time == 0) {
                current->state = FINISHED;
                current->end_time = clock + 1;
                current = NULL;
                completed++;
            }
        }
        clock++;
    }

    printf("Scheduling Policy: Preemptive Shortest Job First\n");
    print_stats(jobs, count, clock);
    free_queue(io);
}

void multilevel_feedback_queue(Job jobs[], int count) {
    Queue *queues[MLFQ_LEVELS];
    for (int i = 0; i < MLFQ_LEVELS; i++) queues[i] = create_queue();
    Queue *io = create_queue();

    int completed = 0, clock = 0;
    Job *current = NULL;
    int time_slice = 0;

    int aging_counters[100] = {0};

    while (completed < count) {
        for (int i = 0; i < count; i++)
            if (jobs[i].arrival_time == clock && jobs[i].state == NEW)
                jobs[i].state = READY, enqueue(queues[0], &jobs[i]);

        int io_jobs = io->count;
        for (int i = 0; i < io_jobs; i++) {
            Job *j = dequeue(io);
            if (IO_complete()) {
                j->state = READY;
                enqueue(queues[j->queue_level], j);
            } else enqueue(io, j);
        }

        for (int i = 0; i < count; i++) {
            if (jobs[i].state == READY) {
                aging_counters[i]++;
                if (aging_counters[i] >= AGING_THRESHOLD && jobs[i].queue_level > 0) {
                    jobs[i].queue_level--;
                    aging_counters[i] = 0;
                }
            }
        }

        if (!current) {
            for (int i = 0; i < MLFQ_LEVELS; i++) {
                if (!is_empty(queues[i])) {
                    current = dequeue(queues[i]);
                    current->state = RUNNING;
                    time_slice = 0;
                    break;
                }
            }
        }

        if (current) {
            current->remaining_time--;
            time_slice++;
            current->total_cpu_time++;

            if (IO_request()) {
                current->state = WAITING;
                enqueue(io, current);
                current = NULL;
            } else if (current->remaining_time == 0) {
                current->state = FINISHED;
                current->end_time = clock + 1;
                current = NULL;
                completed++;
            } else if (time_slice == time_slices[current->queue_level]) {
                if (current->queue_level < MLFQ_LEVELS - 1) current->queue_level++;
                current->state = READY;
                enqueue(queues[current->queue_level], current);
                current = NULL;
            }
        }
        clock++;
    }

    printf("Scheduling Policy: Multi-Level Feedback Queue\n");
    print_stats(jobs, count, clock);
    for (int i = 0; i < MLFQ_LEVELS; i++) free_queue(queues[i]);
    free_queue(io);
}