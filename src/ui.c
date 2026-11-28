#include <stdio.h>
#include "ui.h"

#define BAR_WIDTH 65

static void border(void) 
{
    putchar('+');
    for (int i = 0; i < BAR_WIDTH; i++) putchar('-');
    puts("+");
}

void drawBar(TabList *t) 
{
    if (!t || !t->active) return;
    Tab *a = t->active;
    putchar('\n');
    border();
    printf("| Tab %d  | [%c Back] [%c Fwd] | URL: %-30.30s |\n", a->id,
           stack_empty(&a->back) ? '-' : '<',
           stack_empty(&a->forward) ? '-' : '>',
           a->current);
    border();
}

void help(void) {
    printf("\n  visit <url>      Navigate the active tab to a URL\n"
           "  back             Go to the previous page\n"
           "  forward          Go to the next page\n"
           "  history          Show the Back stack, current page, and Forward stack\n"
           "  newtab           Open a new tab\n"
           "  closetab [id]    Close a tab (the active tab if no id is given)\n"
           "  tabs             List all tabs; the active tab is marked with *\n"
           "  switch <id>      Switch to the tab with the given id\n"
           "  bookmark         Bookmark the current page\n"
           "  bookmarks        List all bookmarks\n"
           "  delbookmark <n>  Delete bookmark number n\n"
           "  download <file>  Add a file to the loading queue\n"
           "  queue            Display the loading queue\n"
           "  process          Process the next file in the queue\n"
           "  help             Display the command list\n"
           "  exit             Quit the program\n");
}