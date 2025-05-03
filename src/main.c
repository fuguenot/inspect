#include "inspect.h"

#include <stdlib.h>

const char *prog_name;

struct buffer_t *bufs[NBUFS];
int buf_idx;

int rows, cols;

bool redraw_display_needed, redraw_bar_needed;
bool running;

int main(int argc, char *const *argv) {
    int idx = process_args(argc, argv);
    for (int i = idx; i < argc; i++) {
        buf_idx = open_buffer(argv[i]);
        if (buf_idx < 0) {
            close_all_buffers();
            exit(1);
        }
    }
    init_ui();
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
