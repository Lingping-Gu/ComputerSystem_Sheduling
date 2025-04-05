#ifndef QUEUE_H
#define QUEUE_H

#include "job.h"

typedef struct Node {
    Job *job;
    struct Node *next;
} Node;

typedef struct {
    Node *front, *rear;
    int count;
} Queue;

Queue* create_queue();
void enqueue(Queue *q, Job *job);
Job* dequeue(Queue *q);
Job* peek(Queue *q);
int is_empty(Queue *q);
void free_queue(Queue *q);

#endif
