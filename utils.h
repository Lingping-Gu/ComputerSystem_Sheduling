#ifndef UTILS_H
#define UTILS_H
#include "job.h"

int os_rand();
void os_srand(unsigned int seed);
int IO_request();
int IO_complete();
void print_stats(Job jobs[], int count, int total_time);

#endif
