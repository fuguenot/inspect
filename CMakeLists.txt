cmake_minimum_required(VERSION 4.0)
project(inspect VERSION 1.0.0 DESCRIPTION "Look at files" LANGUAGES C)
find_package(Curses REQUIRED)
add_executable(inspect-bin
    "src/main.c" "src/inspect.h"
    "src/args.c" "src/render.c"
    "src/input.c" "src/buffer.c"
    "src/file.c" "src/error.h"
    "src/error.c"
)
target_link_libraries(inspect-bin PRIVATE ${CURSES_LIBRARIES})
set_target_properties(inspect-bin PROPERTIES OUTPUT_NAME i)
install(TARGETS inspect-bin RUNTIME DESTINATION bin)
install(DIRECTORY help/ DESTINATION help)
