/*
 * Web Browser Simulator  (Data Structures Final Project)
 * -------------------------------------------------------
 *  Stack         -> Back / Forward history (one pair of stacks per tab)
 *  Linked List   -> Browser tabs (open, close, switch)
 *  Queue         -> File loading / download requests (FIFO)
 *  Dynamic Array -> Bookmarks (grows with realloc, delete shifts items)
 *
 * Compile:  gcc browser_sim.c -o browser_sim
 * Run:      ./browser_sim
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define URL_LEN 128

/* ============================ 1. STACK ============================ */
typedef struct StackNode {
    char url[URL_LEN];
    struct StackNode *next;
} StackNode;

void push(StackNode **top, const char *url) {
    StackNode *n = malloc(sizeof(StackNode));
    strncpy(n->url, url, URL_LEN - 1);
    n->url[URL_LEN - 1] = '\0';
    n->next = *top;
    *top = n;
}

int pop(StackNode **top, char *out) {          /* returns 0 if empty */
    if (*top == NULL) return 0;
    StackNode *n = *top;
    strcpy(out, n->url);
    *top = n->next;
    free(n);
    return 1;
}

void clearStack(StackNode **top) {
    char tmp[URL_LEN];
    while (pop(top, tmp));
}

/* ========================= 2. LINKED LIST (TABS) ========================= */
typedef struct Tab {
    int id;
    char current[URL_LEN];                      /* page being shown */
    StackNode *back;                            /* Back stack */
    StackNode *forward;                         /* Forward stack */
    struct Tab *next;
} Tab;

typedef struct {
    Tab *head;
    Tab *active;
    int nextId;
} TabList;

Tab *openTab(TabList *t) {
    Tab *n = calloc(1, sizeof(Tab));
    n->id = t->nextId++;
    strcpy(n->current, "about:blank");
    if (t->head == NULL) t->head = n;
    else {
        Tab *p = t->head;
        while (p->next) p = p->next;
        p->next = n;
    }
    t->active = n;
    return n;
}

int closeTab(TabList *t, int id) {
    Tab *prev = NULL, *cur = t->head;
    while (cur && cur->id != id) { prev = cur; cur = cur->next; }
    if (!cur) return 0;
    if (prev) prev->next = cur->next; else t->head = cur->next;
    if (t->active == cur)
        t->active = prev ? prev : t->head;
    clearStack(&cur->back);
    clearStack(&cur->forward);
    free(cur);
    if (t->head == NULL) openTab(t);            /* browser never has 0 tabs */
    return 1;
}

int switchTab(TabList *t, int id) {
    for (Tab *p = t->head; p; p = p->next)
        if (p->id == id) { t->active = p; return 1; }
    return 0;
}

void listTabs(TabList *t) {
    printf("\n  Tabs: ");
    for (Tab *p = t->head; p; p = p->next)
        printf("[%s%d: %s]%s", p == t->active ? "*" : "", p->id, p->current,
               p->next ? " -> " : "");
    printf("\n");
}

/* ============================ 3. QUEUE ============================ */
typedef struct QNode {
    char file[URL_LEN];
    struct QNode *next;
} QNode;

typedef struct { QNode *front, *rear; int size; } Queue;

void enqueue(Queue *q, const char *file) {
    QNode *n = malloc(sizeof(QNode));
    strncpy(n->file, file, URL_LEN - 1);
    n->file[URL_LEN - 1] = '\0';
    n->next = NULL;
    if (q->rear) q->rear->next = n; else q->front = n;
    q->rear = n;
    q->size++;
}

int dequeue(Queue *q, char *out) {
    if (!q->front) return 0;
    QNode *n = q->front;
    strcpy(out, n->file);
    q->front = n->next;
    if (!q->front) q->rear = NULL;
    free(n);
    q->size--;
    return 1;
}

void processOne(Queue *q) {
    char f[URL_LEN];
    if (!dequeue(q, f)) { printf("  (queue is empty)\n"); return; }
    printf("  Loading %-28s [", f);
    for (int i = 0; i < 10; i++) { putchar('#'); }
    printf("] done\n");
}

void processAll(Queue *q) {
    if (q->size == 0) { printf("  (queue is empty)\n"); return; }
    while (q->size > 0) processOne(q);
}

void showQueue(Queue *q) {
    if (!q->front) { printf("  Queue is empty.\n"); return; }
    printf("  FRONT -> ");
    for (QNode *p = q->front; p; p = p->next) printf("%s%s", p->file, p->next ? " -> " : "");
    printf(" <- REAR  (%d waiting)\n", q->size);
}

/* ====================== 4. DYNAMIC ARRAY (BOOKMARKS) ====================== */
typedef struct {
    char (*items)[URL_LEN];
    int size, capacity;
} Bookmarks;

void bmInit(Bookmarks *b) {
    b->capacity = 2;                            /* tiny so you can see it grow */
    b->size = 0;
    b->items = malloc(b->capacity * URL_LEN);
}

int bmAdd(Bookmarks *b, const char *url) {
    for (int i = 0; i < b->size; i++)
        if (strcmp(b->items[i], url) == 0) return 0;     /* duplicate */
    if (b->size == b->capacity) {
        b->capacity *= 2;
        b->items = realloc(b->items, b->capacity * URL_LEN);
        printf("  (array grew: capacity is now %d)\n", b->capacity);
    }
    strncpy(b->items[b->size], url, URL_LEN - 1);
    b->items[b->size][URL_LEN - 1] = '\0';
    b->size++;
    return 1;
}

int bmDelete(Bookmarks *b, int index) {         /* index is 1-based */
    if (index < 1 || index > b->size) return 0;
    for (int i = index - 1; i < b->size - 1; i++)         /* shift left */
        strcpy(b->items[i], b->items[i + 1]);
    b->size--;
    return 1;
}

void bmShow(Bookmarks *b) {
    if (b->size == 0) { printf("  No bookmarks yet.\n"); return; }
    for (int i = 0; i < b->size; i++) printf("  %d. * %s\n", i + 1, b->items[i]);
}

/* ============================ BROWSER LOGIC ============================ */
void visit(TabList *t, Queue *q, const char *url) {
    Tab *tab = t->active;
    if (strcmp(tab->current, "about:blank") != 0)
        push(&tab->back, tab->current);         /* remember where we were */
    clearStack(&tab->forward);                  /* new path erases forward history */
    strcpy(tab->current, url);

    char f[URL_LEN];                            /* a page needs these files */
    snprintf(f, URL_LEN, "%s/index.html", url);  enqueue(q, f);
    snprintf(f, URL_LEN, "%s/style.css", url);   enqueue(q, f);
    snprintf(f, URL_LEN, "%s/script.js", url);   enqueue(q, f);
    printf("  Opening %s ...\n", url);
    processAll(q);
}

void goBack(TabList *t) {
    Tab *tab = t->active;
    char prev[URL_LEN];
    if (!pop(&tab->back, prev)) { printf("  Can't go back: no previous page.\n"); return; }
    push(&tab->forward, tab->current);
    strcpy(tab->current, prev);
    printf("  <- Back to %s\n", tab->current);
}

void goForward(TabList *t) {
    Tab *tab = t->active;
    char next[URL_LEN];
    if (!pop(&tab->forward, next)) { printf("  Can't go forward: no next page.\n"); return; }
    push(&tab->back, tab->current);
    strcpy(tab->current, next);
    printf("  -> Forward to %s\n", tab->current);
}

void drawBar(TabList *t) {
    Tab *a = t->active;
    printf("\n+----------------------------------------------------------+\n");
    printf("| Tab %d  | [%c Back] [%c Fwd] | URL: %-30s |\n", a->id,
           a->back ? '<' : '-', a->forward ? '>' : '-', a->current);
    printf("+----------------------------------------------------------+\n");
}

void help(void) {
    printf("\n  visit <url>      go to a website         back / forward\n"
           "  newtab           open a new tab          closetab [id]\n"
           "  tabs             list tabs               switch <id>\n"
           "  bookmark         save current page       bookmarks\n"
           "  delbookmark <n>  delete bookmark n       download <file>\n"
           "  queue            show loading queue      process (load next file)\n"
           "  history          show back/forward stacks\n"
           "  help             this menu               exit\n");
}

void showHistory(Tab *t) {
    printf("  Back stack (top first):    ");
    for (StackNode *p = t->back; p; p = p->next) printf("%s ", p->url);
    printf("\n  Current:                   %s\n  Forward stack (top first): ", t->current);
    for (StackNode *p = t->forward; p; p = p->next) printf("%s ", p->url);
    printf("\n");
}

int main(void) {
    TabList tabs = {0};
    Queue q = {0};
    Bookmarks bm;
    bmInit(&bm);
    openTab(&tabs);

    char line[256], cmd[32], arg[URL_LEN];
    printf("=== Web Browser Simulator ===  (type 'help')\n");

    while (1) {
        drawBar(&tabs);
        printf("browser> ");
        if (!fgets(line, sizeof line, stdin)) break;
        cmd[0] = arg[0] = '\0';
        sscanf(line, "%31s %127s", cmd, arg);
        if (cmd[0] == '\0') continue;

        if (!strcmp(cmd, "visit")) {
            if (arg[0]) visit(&tabs, &q, arg); else printf("  Usage: visit <url>\n");
        } else if (!strcmp(cmd, "back")) goBack(&tabs);
        else if (!strcmp(cmd, "forward")) goForward(&tabs);
        else if (!strcmp(cmd, "newtab")) { Tab *n = openTab(&tabs); printf("  Opened tab %d\n", n->id); }
        else if (!strcmp(cmd, "closetab")) {
            int id = arg[0] ? atoi(arg) : tabs.active->id;
            if (closeTab(&tabs, id)) printf("  Closed tab %d\n", id); else printf("  No such tab.\n");
        } else if (!strcmp(cmd, "tabs")) listTabs(&tabs);
        else if (!strcmp(cmd, "switch")) {
            if (switchTab(&tabs, atoi(arg))) printf("  Switched to tab %s\n", arg); else printf("  No such tab.\n");
        } else if (!strcmp(cmd, "bookmark")) {
            if (!strcmp(tabs.active->current, "about:blank")) printf("  Nothing to bookmark.\n");
            else if (bmAdd(&bm, tabs.active->current)) printf("  * Bookmarked %s\n", tabs.active->current);
            else printf("  Already bookmarked.\n");
        } else if (!strcmp(cmd, "bookmarks")) bmShow(&bm);
        else if (!strcmp(cmd, "delbookmark")) {
            if (bmDelete(&bm, atoi(arg))) printf("  Deleted.\n"); else printf("  Invalid number.\n");
        } else if (!strcmp(cmd, "download")) {
            if (arg[0]) { enqueue(&q, arg); printf("  Queued %s\n", arg); } else printf("  Usage: download <file>\n");
        } else if (!strcmp(cmd, "queue")) showQueue(&q);
        else if (!strcmp(cmd, "process")) processOne(&q);
        else if (!strcmp(cmd, "history")) showHistory(tabs.active);
        else if (!strcmp(cmd, "help")) help();
        else if (!strcmp(cmd, "exit")) break;
        else printf("  Unknown command. Type 'help'.\n");
    }
    printf("Goodbye!\n");
    return 0;
}
