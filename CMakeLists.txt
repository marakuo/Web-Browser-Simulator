cmake_minimum_required(VERSION 3.20)
project(Web_Browser_Simulator LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(Web_Browser_Simulator
        src/main.c
        src/browser.c
        src/tabs.c
        src/stack.c
        src/queue.c
        src/bookmarks.c
)

target_include_directories(Web_Browser_Simulator PRIVATE include)