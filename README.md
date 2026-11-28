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
8. [Course Information](#course-information)

---

## Project Objectives

- Apply stacks, linked lists, queues, and dynamic arrays to a realistic, easily understood problem.
- Practice dynamic memory management in C (`malloc`, `realloc`, `free`).
- Practice modular programming: separating code into header and source files with clear interfaces.
- Practice collaborative software development using Git and a shared repository.
- Learn project configuration and build management using CMake.

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

- Visiting a page enqueues `index.html`, `style.css`, and `script.js` for the visited URL.
- Files are processed in FIFO order.
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

```text
web-browser-simulator/
|
|-- README.md               Project description (this file)
|-- TASKS.md                Task breakdown and team assignments
|-- CMakeLists.txt          CMake build configuration
|-- reference/
|   `-- browser_sim.c       Single-file sample implementation
|-- include/
|   |-- common.h            Shared constants (for example URL_LEN)
|   |-- stack.h
|   |-- tabs.h
|   |-- queue.h
|   |-- loader.h            Page loading and download processing
|   |-- bookmarks.h
|   |-- browser.h
|   `-- ui.h                Address bar and help menu
|-- src/
|   |-- stack.c
|   |-- tabs.c
|   |-- queue.c
|   |-- loader.c
|   |-- bookmarks.c
|   |-- browser.c           Navigation logic (visit, back, forward)
|   |-- ui.c                drawBar and help
|   `-- main.c              Command loop
|-- tests/
|   `-- test_*.c            Unit tests for each module
|-- docs/
|   |-- report.md           Final project report
|   `-- diagrams/           Data structure and flow diagrams
`-- web/                    Optional HTML/CSS front end
```

---

## Build and Run

### Requirements

- GCC or any C99-compatible C compiler.
- CMake (version 3.20 or later).
- A compatible build tool, such as Make or Ninja.

### Build Using CMake

**1. Clone the repository**

```bash
git clone <repository-url>
cd web-browser-simulator
```

**2. Configure the project**

```bash
cmake -S . -B build
```

**3. Build the project**

```bash
cmake --build build
```

**4. Run the executable**

On Linux and macOS:

```bash
./build/browser_sim
```

On Windows:

```powershell
.\build\Debug\browser_sim.exe
```

*Note: The executable location on Windows may vary depending on the selected CMake generator and build configuration.*

### Build Using CLion

1. Clone or open the repository in CLion.
2. CLion will detect the `CMakeLists.txt` file automatically.
3. Wait for CMake project configuration to finish.
4. Select the `browser_sim` run configuration.
5. Click Run to build and execute the project.

### Compile with Warnings Enabled

During development, use the following command to enable compiler warnings:

```bash
gcc -Wall -Wextra -std=c99 -g src/*.c -Iinclude -o browser_sim
```

This helps identify potential issues during development.

---

## Design Overview

### Tab

Each tab stores its identifier, the page currently displayed, a Back stack, a Forward stack, and a pointer to the next tab.

```c
Tab {
    id,
    current,
    back*,
    forward*,
    next*
}
```

### Navigation Rules

1. `visit`: Push the current page to Back (unless it is `about:blank`), clear Forward, and set the new page as current.
2. `back`: Pop from Back, push the current page to Forward, and set the popped page as current.
3. `forward`: Pop from Forward, push the current page to Back, and set the popped page as current.

### Loading Rules

1. Each visit enqueues `index.html`, `style.css`, and `script.js` for the visited URL.
2. Files are dequeued and "loaded" strictly in arrival order.

### Bookmark Rules

1. Initial capacity is small (2) so growth is easy to observe during demonstrations.
2. Capacity doubles when full.
3. Deleting item `i` shifts all later items one position to the left.

---

## Course Information

- **Course:** Data Structures
- **Institution:** Mansoura University, Faculty of Engineering
- **Department:** Computer and Control Systems
- **Programming Language:** C
- **Build System:** CMake
