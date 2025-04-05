#include "job.h"

void init_job_stats(Job *job) {
    for (int i = 0; i < 5; ++i)
        job->time_in_state[i] = 0;
    job->end_time = 0;
    job->last_start_time = -1;
    job->total_waiting_time = 0;
    job->total_io_time = 0;
    job->total_cpu_time = 0;
    job->queue_level = 0;
}
