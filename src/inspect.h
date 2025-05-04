#pragma once

#include <stdbool.h>
#include <stdio.h>

#define INSPECT_VERSION "1.0.0"

extern const char *prog_name;
extern char *help_path;
extern bool glob_readonly;

#define NBUFS 10
struct buffer_t {
    char *filename;
    char **lines;
    int64_t nlines;
    int drow, dcol, row, col;
    bool readonly;
};
extern struct buffer_t *bufs[NBUFS];
extern int buf_idx;

extern int rows, cols;

extern bool redraw_display_needed, redraw_bar_needed;
extern bool running;

void usage(FILE *out);
void help();
int process_args(int argc, char *const *argv);

int load_file(const char *name, struct buffer_t *buf);

int open_buffer(const char *name, bool readonly, const char *path);
void close_buffer(int idx);
void close_all_buffers();
int buffer_count();
bool find_next_buffer(bool left, bool right);

void init_ui();
void cleanup();

void redraw(bool force);
void resize();

int handle_events();
