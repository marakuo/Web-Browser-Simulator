#include<stdio.h>
#include<stdlib.h>
#include "tabs.h"
// tabs counter
int count_Tabs(TabList *t) {
    if (t == NULL) return 0;
    Tab *temp = t->head;
    int ctr =0;
    while (temp != NULL) {
        ctr++;
        temp = temp->next;
    }
    return ctr;

}
// list tabs
void list_Tabs(TabList *t) {
    // no tabs or empty tablist
    if (t == NULL || t->head == NULL) {
        printf("No Tabs open\n");return;
    }
    Tab *temp = t->head;
    printf("\n tabs\n");
    while (temp != NULL) {
        // if this tab is active put * before
        if (temp  == t->active) {
            printf("  *[%d]%s\n", temp->id, temp->current);
        }else {
            printf("   [%d]%s\n", temp->id, temp->current);
        }
        temp = temp->next; // move to the next tab (for every tab)
    }
}


// close tabs returns int to check if the process is correctly done
int close_Tab(TabList *t, int id) {
    if (t == NULL || t->head == NULL) return 0;
    Tab *temp = t->head; // the one to delete
    Tab *prev = NULL; // the one before
    while (temp !=NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }
    if (temp ==NULL) return 0; // id not found

    if (prev == NULL) // deleting the 1st node
        t->head = temp->next;
    else // middle or last node
        prev->next = temp ->next;

    // active tabs is the one being removed
    if (t->active == temp) {
        // move active to the tab before it, or to the new first tab
        if (prev != NULL) t->active = prev;
        else t->active = t->head;

    }
    free(temp);
    // if the only tab is deleted the browser opens new empty tab
    // keep at least new tab
    if (t->head == NULL)  openTab(t); // member 3



    return 1;

}

// free all tabs
void freeAllTabs(TabList *t) {
    if (t == NULL) return;
    Tab *temp = t->head; // 1st node
    while (temp != NULL) {
        Tab *next = temp->next; // save next address before freee
        free(temp);
        temp = next;
    }
    t->head = NULL;
    t->active = NULL;
}

/* TEMPORARY - delete when Member 3's openTab is added */
Tab *openTab(TabList *t) {
    (void)t;
    return NULL;
}