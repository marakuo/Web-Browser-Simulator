# Task Breakdown (10 Members)

Every task below is coding work on the terminal version. The reference implementation (`reference/browser_sim.c`) already contains working code for each function, so each member copies the relevant part into their own file, adapts it to the signatures below, and makes it compile on its own.

**Golden rule: each member edits only the files they own.** This avoids merge conflicts.

---

## 1. How the code is split

Each module has a header (`.h`) with the definitions and a source (`.c`) with the implementation. Other files only `#include` the header.

**What goes in a header:** `struct` and `typedef` definitions, `#define` constants, function prototypes, and include guards (`#ifndef` / `#define` / `#endif`).
**What goes in the `.c` file:** the function bodies.

Structs must be defined in the headers (not hidden in the `.c`), because other modules read their fields (for example `tab->back`).

### Include dependencies

```
common.h  <-- stack.h  <-- tabs.h  <-- browser.h
          <-- queue.h  <-- loader.h  <-- browser.h
          <-- bookmarks.h
          <-- ui.h  (uses tabs.h)
```

`main.c` includes everything.

---

## 2. The contract (function signatures)

Member 1 writes these headers first and pushes them, so everyone can code against the same declarations from day one.

**common.h**
```c
#define URL_LEN 128
/* includes: stdio.h, stdlib.h, string.h */
```

**stack.h**
```c
typedef struct StackNode { char url[URL_LEN]; struct StackNode *next; } StackNode;

void push(StackNode **top, const char *url);
int  pop(StackNode **top, char *out);            /* 1 = success, 0 = empty */
void clearStack(StackNode **top);
int  stackIsEmpty(const StackNode *top);
int  stackSize(const StackNode *top);
const char *stackPeek(const StackNode *top);     /* NULL if empty */
void stackPrint(const StackNode *top);           /* top first, one line */
```

**tabs.h**
```c
typedef struct Tab {
    int id;
    char current[URL_LEN];
    StackNode *back;
    StackNode *forward;
    struct Tab *next;
} Tab;
typedef struct { Tab *head; Tab *active; int nextId; } TabList;

void tabsInit(TabList *t);
Tab *openTab(TabList *t);
int  switchTab(TabList *t, int id);              /* 1 = ok, 0 = no such tab */
int  countTabs(const TabList *t);
void listTabs(const TabList *t);
int  closeTab(TabList *t, int id);               /* 1 = ok, 0 = no such tab */
void freeAllTabs(TabList *t);
```

**queue.h**
```c
typedef struct QNode { char file[URL_LEN]; struct QNode *next; } QNode;
typedef struct { QNode *front, *rear; int size; } Queue;

void queueInit(Queue *q);
void enqueue(Queue *q, const char *file);
int  dequeue(Queue *q, char *out);               /* 1 = success, 0 = empty */
int  queueIsEmpty(const Queue *q);
void clearQueue(Queue *q);
```

**loader.h**
```c
void enqueuePageFiles(Queue *q, const char *url);   /* index.html, style.css, script.js */
void processOne(Queue *q);
void processAll(Queue *q);
void showQueue(const Queue *q);
```

**bookmarks.h**
```c
typedef struct { char (*items)[URL_LEN]; int size, capacity; } Bookmarks;

void bmInit(Bookmarks *b);
int  bmAdd(Bookmarks *b, const char *url);       /* 1 = added, 0 = duplicate */
int  bmFind(const Bookmarks *b, const char *url);/* index, or -1 */
void bmFree(Bookmarks *b);
int  bmDelete(Bookmarks *b, int index);          /* index is 1-based; 1 = ok, 0 = invalid */
void bmShow(const Bookmarks *b);
```

**browser.h**
```c
void visit(TabList *t, Queue *q, const char *url);
void goBack(TabList *t);
void goForward(TabList *t);
```

**ui.h**
```c
void drawBar(const TabList *t);
void help(void);
void showHistory(const Tab *t);
```

---

## 3. Member assignments

### Member 1: Foundation and Stack core
**Files:** `include/common.h`, `include/stack.h`, `include/tabs.h`, `include/queue.h`, `include/loader.h`, `include/bookmarks.h`, `include/browser.h`, `include/ui.h`, `src/stack.c`
**Do first:** Write all headers exactly as in section 2 and push them.
**Then implement:** `push`, `pop`, `clearStack` (linked-list stack: push and pop at the head, `pop` returns 0 on empty).
**Called by:** almost everyone.

### Member 2: Stack utilities and display
**Files:** `src/stack_utils.c`, `src/ui.c`
**Implement:**
- `stackIsEmpty`, `stackSize`, `stackPeek`, `stackPrint`
- `drawBar` (prints the box with tab id, Back/Forward availability, current URL)
- `help` (prints the command list)
- `showHistory` (prints Back stack, current page, Forward stack using `stackPrint`)

### Member 3: Tabs (open, switch, list)
**Files:** `src/tabs.c`
**Implement:** `tabsInit`, `openTab` (append a node to the end of the list, new tab starts at `about:blank` and becomes active), `switchTab`, `countTabs`, `listTabs` (active tab marked with `*`).

### Member 4: Tabs (close and cleanup)
**Files:** `src/tabs_close.c`
**Implement:**
- `closeTab`: find the node, unlink it (head case and middle case), pick a new active tab if the active one was closed, free its Back and Forward stacks, free the node, and open a fresh tab if none are left.
- `freeAllTabs`: free every tab and its two stacks (used on exit).

### Member 5: Queue core
**Files:** `src/queue.c`
**Implement:** `queueInit`, `enqueue` (add at rear), `dequeue` (remove from front, reset `rear` to NULL when empty), `queueIsEmpty`, `clearQueue`.

### Member 6: Loader (file loading logic)
**Files:** `src/loader.c`
**Implement:**
- `enqueuePageFiles`: builds `<url>/index.html`, `<url>/style.css`, `<url>/script.js` and enqueues them in that order.
- `processOne`: dequeue one file and print the loading progress bar line.
- `processAll`: process until the queue is empty (print a message if it was already empty).
- `showQueue`: print `FRONT -> a -> b -> c <- REAR (n waiting)`.

### Member 7: Bookmarks (create, add, find, free)
**Files:** `src/bookmarks.c`
**Implement:**
- `bmInit`: capacity 2, size 0, allocate with `malloc`.
- `bmAdd`: reject duplicates (use `bmFind`), double the capacity with `realloc` when full and print the "array grew" message, then store the URL.
- `bmFind`, `bmFree`.

### Member 8: Bookmarks (delete and show)
**Files:** `src/bookmarks_ops.c`
**Implement:**
- `bmDelete`: validate the 1-based index, shift all later items one position left, decrease `size`.
- `bmShow`: print numbered bookmarks, or "No bookmarks yet."

### Member 9: Browser navigation logic
**Files:** `src/browser.c`
**Implement:**
- `visit`: push the current page to Back (unless `about:blank`), clear Forward, set the new current page, call `enqueuePageFiles`, print "Opening ...", then `processAll`.
- `goBack`: pop from Back, push the current page onto Forward, update current. Print a message if Back is empty.
- `goForward`: the mirror of `goBack`.

### Member 10: Main program and build
**Files:** `src/main.c`, `Makefile`
**Implement:**
- Create and initialize `TabList`, `Queue`, `Bookmarks`, and open the first tab.
- The command loop: `fgets`, `sscanf` into command and argument, `drawBar` before each prompt.
- Dispatch every command in the README's command table to the right function. Check for missing arguments (`visit` with no URL, and so on).
- On `exit`: call `freeAllTabs`, `clearQueue` and `bmFree` before quitting.
- `Makefile` that compiles every file in `src/` with `-Wall -Wextra -std=c99 -Iinclude`.

---

## 4. Work order

| Stage | Who | What |
|---|---|---|
| 1 | Member 1 | Push all headers (section 2). |
| 2 | Members 1 to 9 | Implement their own `.c` files in parallel. Nobody waits for anyone, because everyone codes against the headers. |
| 3 | Member 10 | Writes `main.c` against the headers at the same time, then links everything together once the others are merged. |

---

## 5. Optional extras (linear data structures only)

Take these after your main task is merged.

| Extra | Data structure | Suggested owner |
|---|---|---|
| `reopen` command: bring back the last closed tab | A stack of closed-tab URLs | Member 4 (store on close) and Member 10 (command) |
| Save bookmarks to a file on exit and load them on start | Dynamic array + file I/O | Member 8 |
| `clearhistory` command for the active tab | Stack | Member 2 |
