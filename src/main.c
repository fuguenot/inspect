#include "inspect.h"

#include <stdlib.h>

#include "error.h"

const char *prog_name;
char *help_path;

struct buffer_t *bufs[NBUFS];
int buf_idx;

int rows, cols;

bool redraw_display_needed, redraw_bar_needed;
bool running;

int main(int argc, char *const *argv) {
    int idx = process_args(argc, argv);
    if (idx == RET_ERR) {
        print_errmsg();
        free_errmsg();
        exit(1);
    }
    for (int i = idx; i < argc && buffer_count() < NBUFS; i++) {
        buf_idx = open_buffer(argv[i], false, NULL);
        if (buf_idx == RET_ERR) {
            print_errmsg();
            free_errmsg();
            close_all_buffers();
            exit(1);
        }
    }
    init_ui();
    redraw(1);
    while (running) {
        redraw(0);
        if (handle_events() == RET_ERR) {
            switch (error.code) {
            case E_BUFS_FULL:
            case E_FILE_NOT_FOUND: buf_idx = 0; break;
            default:
                print_errmsg();
                free_errmsg();
                cleanup();
                close_all_buffers();
                exit(1);
                break;
            }
        }
    }

    cleanup();
    close_all_buffers();
    free(help_path);
    free_errmsg();
}
