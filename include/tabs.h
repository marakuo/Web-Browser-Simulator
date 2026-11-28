
#ifndef WEB_BROWSER_SIMULATOR_TABS_H
#define WEB_BROWSER_SIMULATOR_TABS_H
#include<stack.h>
typedef struct Tab {
    int id;
    char current[max_url_len]; // tab url
    Stack back;
    Stack forward;
    struct Tab *next;
} Tab;
typedef struct {
    Tab *head;
    Tab *active;
    int nextId;
} TabList;

/* Member 3: tabs creation */

Tab *openTab(TabList *t); /* creates a new tab and makes it active */

/* Member 4: tabs management */

int  count_Tabs(TabList *t);  /* returns the number of open tabs */
void list_Tabs(TabList *t);  /* prints all tabs, * marks the active one */
int  close_Tab(TabList *t, int id); /* 1 = closed, 0 = id not found; reopens a tab if none are left */
void freeAllTabs(TabList *t);  /* frees every tab, call once when the program exits */


#endif //WEB_BROWSER_SIMULATOR_TABS_H
