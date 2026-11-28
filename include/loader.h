#ifndef WEB_BROWSER_SIMULATOR_LOADER_H
#define WEB_BROWSER_SIMULATOR_LOADER_H

#include "queue.h"

void enqueuePageFiles(Queue *q, const char *url);
void processOne(Queue *q);
void processAll(Queue *q);
void showQueue(const Queue *q);

#endif