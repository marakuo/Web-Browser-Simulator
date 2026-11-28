#ifndef WEB_BROWSER_SIMULATOR_QUEUE_H
#define WEB_BROWSER_SIMULATOR_QUEUE_H

#include <stdbool.h>

#define QUEUE_URL_LEN 272

typedef struct QNode {
    char file[QUEUE_URL_LEN];
    struct QNode *next;
} QNode;

typedef struct {
    QNode *front;
    QNode *rear;
    int size;
} Queue;

void queue_init(Queue *q);
bool queue_empty(const Queue *q);
int queue_size(const Queue *q);
bool enqueue(Queue *q, const char *file);
bool dequeue(Queue *q, char *out);
bool queue_peek(const Queue *q, char *out);
void queue_clear(Queue *q);

#endif