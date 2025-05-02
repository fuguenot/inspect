#include "inspect.h"

const char *prog_name;

const char *file;
char **lines;
int64_t clines;

int rows, cols;

int redraw_display_needed, redraw_bar_needed;

int running;

int drow, dcol;
int row, col;

int main(int argc, char *const *argv) {
    process_args(argc, argv);
    load_file();
    init();
    redraw(1);
    while (running) {
        redraw(0);
        handle_events();
    }

    cleanup();
}
