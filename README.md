# Web Browser Simulator

A console-based web browser simulator written in C, developed as the final project for the Data Structures course at the Computer and Control Systems Department, Faculty of Engineering, Mansoura University.

The project models the core behaviour of a web browser (navigation history, tabs, file loading, and bookmarks) in order to demonstrate practical use of the four fundamental data structures covered in the course.

---

## Table of Contents

1. [Project Objectives](#project-objectives)
2. [Data Structures Used](#data-structures-used)
3. [Features](#features)
4. [Command Reference](#command-reference)
5. [Repository Structure](#repository-structure)
6. [Build and Run](#build-and-run)
7. [Design Overview](#design-overview)
8. [Team Workflow](#team-workflow)
9. [Coding Standards](#coding-standards)
10. [Task Distribution](#task-distribution)

---

## Project Objectives

- Apply stacks, linked lists, queues, and dynamic arrays to a realistic, easily understood problem.
- Practice dynamic memory management in C (`malloc`, `realloc`, `free`).
- Practice modular programming: separating code into header and source files with clear interfaces.
- Practice collaborative software development using Git and a shared repository.

---

## Data Structures Used

| Data Structure | Browser Feature | Description |
|---|---|---|
| Stack | Back and Forward history | Each tab owns two stacks. Visiting a page pushes the current page onto the Back stack. Going back pops from Back and pushes onto Forward. |
| Singly Linked List | Browser tabs | Tabs are nodes in a list. Opening a tab appends a node, closing a tab removes a node, switching changes the active pointer. |
| Queue (FIFO) | Page loading and downloads | Files requested by a page (HTML, CSS, JavaScript) and manual downloads are processed in the order they were requested. |
| Dynamic Array | Bookmarks | A resizable array that doubles its capacity with `realloc` when full. Deleting a bookmark shifts the remaining items left. |

---

## Features

**Navigation**
- Visit a URL in the active tab.
- Move back and forward through the history of the active tab.
- Visiting a new page clears the forward history, as in a real browser.
- Inspect the Back and Forward stacks at any time.

**Tabs**
- Open, close, list, and switch between tabs.
- Each tab maintains its own independent history.
- The browser always keeps at least one tab open.

**Loading and Downloads**
- Visiting a page enqueues its resource files and processes them in FIFO order.
- Files can be queued manually and processed one at a time.
- A progress indicator is displayed for each file.

**Bookmarks**
- Save the current page as a bookmark (duplicates are rejected).
- List and delete bookmarks by number.
- The underlying array grows automatically when its capacity is reached.

---

## Command Reference

| Command | Description |
|---|---|
| `visit <url>` | Navigate the active tab to a URL |
| `back` | Go to the previous page |
| `forward` | Go to the next page |
| `history` | Show the Back stack, current page, and Forward stack |
| `newtab` | Open a new tab |
| `closetab [id]` | Close a tab (the active tab if no id is given) |
| `tabs` | List all tabs; the active tab is marked with `*` |
| `switch <id>` | Switch to the tab with the given id |
| `bookmark` | Bookmark the current page |
| `bookmarks` | List all bookmarks |
| `delbookmark <n>` | Delete bookmark number `n` |
| `download <file>` | Add a file to the loading queue |
| `queue` | Display the loading queue |
| `process` | Process the next file in the queue |
| `help` | Display the command list |
| `exit` | Quit the program |

---

## Repository Structure

The reference implementation is a single file. The team will split it into modules so that members can work in parallel without editing the same file.

```
web-browser-simulator/
|-- README.md               Project description (this file)
|-- TASKS.md                Task breakdown and team assignments
|-- Makefile                Build instructions
|-- reference/
|   `-- browser_sim.c       Single-file sample implementation
|-- include/
|   |-- common.h            Shared constants (for example URL_LEN)
|   |-- stack.h
|   |-- tabs.h
|   |-- queue.h
|   |-- bookmarks.h
|   `-- browser.h
|-- src/
|   |-- stack.c
|   |-- tabs.c
|   |-- queue.c
|   |-- bookmarks.c
|   |-- browser.c           Navigation logic (visit, back, forward)
|   `-- main.c              Command loop and user interface
|-- tests/
|   `-- test_*.c            Unit tests for each module
|-- docs/
|   |-- report.md           Final project report
|   `-- diagrams/           Data structure and flow diagrams
`-- web/                    Optional HTML/CSS front end
```

---

## Build and Run

Requirements: GCC (or any C99-compatible compiler) and Make.

Build the reference implementation:

```
gcc reference/browser_sim.c -o browser_sim
./browser_sim
```

Build the modular version (once the Makefile is in place):

```
make
./browser_sim
```

Compile with warnings enabled during development:

```
gcc -Wall -Wextra -std=c99 -g src/*.c -Iinclude -o browser_sim
```

---

## Design Overview

**Tab**

Each tab stores its identifier, the page currently displayed, a Back stack, a Forward stack, and a pointer to the next tab.

```
Tab { id, current, back*, forward*, next* }
```

**Navigation rules**

1. `visit`: push the current page to Back (unless it is `about:blank`), clear Forward, set the new page as current.
2. `back`: pop from Back, push the current page to Forward, set the popped page as current.
3. `forward`: pop from Forward, push the current page to Back, set the popped page as current.

**Loading rules**

1. Each visit enqueues `index.html`, `style.css`, and `script.js` for the visited URL.
2. Files are dequeued and "loaded" strictly in arrival order.

**Bookmark rules**

1. Initial capacity is small (2) so growth is easy to observe during demonstrations.
2. Capacity doubles when full.
3. Deleting item `i` shifts all later items one position to the left.

---


## Course Information

- Course: Data Structures
- Institution: Mansoura University, Faculty of Engineering
- Department: Computer and Control Systems
- Language: C
