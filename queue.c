#include <stdlib.h>
#include "queue.h"

Queue* create_queue() {
    Queue *q = malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    q->count = 0;
    return q;
}

void enqueue(Queue *q, Job *job) {
    Node *new_node = malloc(sizeof(Node));
    new_node->job = job;
    new_node->next = NULL;
    if (q->rear) q->rear->next = new_node;
    else q->front = new_node;
    q->rear = new_node;
    q->count++;
}

Job* dequeue(Queue *q) {
    if (!q->front) return NULL;
    Node *temp = q->front;
    Job *job = temp->job;
    q->front = temp->next;
    if (!q->front) q->rear = NULL;
    free(temp);
    q->count--;
    return job;
}

Job* peek(Queue *q) {
    return q->front ? q->front->job : NULL;
}

int is_empty(Queue *q) {
    return q->count == 0;
}

void free_queue(Queue *q) {
    while (!is_empty(q)) dequeue(q);
    free(q);
}
