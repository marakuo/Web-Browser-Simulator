#include <stdio.h>
#include "loader.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <time.h>
#endif

static void delay_ms(int ms) 
{
#ifdef _WIN32
    Sleep(ms);
#else
    struct timespec ts = {
        ms / 1000,
        (long)(ms % 1000) * 1000000L
    };

    nanosleep(&ts, NULL);
#endif
}

void enqueuePageFiles(Queue *q, const char *url) {
    char f[QUEUE_URL_LEN];
    snprintf(f, QUEUE_URL_LEN, "%s/index.html", url);
    enqueue(q, f);
    snprintf(f, QUEUE_URL_LEN, "%s/style.css", url);
    enqueue(q, f);
    snprintf(f, QUEUE_URL_LEN, "%s/script.js", url);
    enqueue(q, f);
}

void processOne(Queue *q) {
    char f[QUEUE_URL_LEN];
    if (!dequeue(q, f))
    {
        printf("  (queue is empty)\n");
        return;
    }
    printf("  Loading %-28s [", f);
    fflush(stdout);
    for (int i = 0; i < 10; i++)
    {
        putchar('#');
        fflush(stdout);
        delay_ms(30);
    }
    printf("] done\n");
}

void processAll(Queue *q) {
    if (q->size == 0) {
        printf("  (queue is empty)\n");
        return;
    }
    while (q->size > 0) processOne(q);
}

void showQueue(const Queue *q) {
    if (!q->front) {
        printf("  Queue is empty.\n");
        return;
    }
    printf("  FRONT -> ");
    for (QNode *p = q->front; p; p = p->next)
        printf("%s%s", p->file, p->next ? " -> " : "");
    printf(" <- REAR  (%d waiting)\n", q->size);
}