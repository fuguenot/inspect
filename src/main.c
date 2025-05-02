#include "inspect.h"

#include <stdlib.h>

const char *prog_name;

struct buffer_t *buffers[10];
int buf_idx;

int rows, cols;

int redraw_display_needed, redraw_bar_needed;

int running;

int drow, dcol;
int row, col;

int main(int argc, char *const *argv) {
    char *file = process_args(argc, argv);
    buf_idx = open_buffer(file);
    if (buf_idx < 0) exit(1);
    init();
    redraw(1);
    while (running) {
        redraw(0);
        if (handle_events() < 0) {
            cleanup();
            close_all_buffers();
            exit(1);
        }
    }

    cleanup();
    close_all_buffers();
}
