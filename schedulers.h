#ifndef SCHEDULERS_H
#define SCHEDULERS_H

#include "job.h"

void round_robin(Job jobs[], int count);
void shortest_job_first(Job jobs[], int count);
void multilevel_feedback_queue(Job jobs[], int count);

#endif
