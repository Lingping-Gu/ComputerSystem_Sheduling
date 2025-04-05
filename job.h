#ifndef JOB_H
#define JOB_H

typedef enum { NEW, READY, RUNNING, WAITING, FINISHED } State;

typedef struct {
    int pid;
    int arrival_time;
    int service_time;
    int remaining_time;
    int priority;
    int queue_level;
    int time_in_state[5];
    int end_time;
    int last_start_time;
    int total_waiting_time;
    int total_io_time;
    int total_cpu_time;
    State state;
} Job;

void init_job_stats(Job *job);

#endif
