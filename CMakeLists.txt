cmake_minimum_required(VERSION 4.0)
project(inspect VERSION 0.4.0 DESCRIPTION "Look at files" LANGUAGES C)
find_package(Curses REQUIRED)
add_executable(inspect
    "src/main.c" "src/inspect.h"
    "src/args.c" "src/render.c"
    "src/input.c" "src/buffer.c"
)
target_link_libraries(inspect PRIVATE ${CURSES_LIBRARIES})
