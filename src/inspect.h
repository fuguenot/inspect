#pragma once

#include <stdio.h>

#define E_ALLOC 0x0001
#define E_IO 0x0002
#define E_FULL 0x0003
#define E_BUFFER 0x0100
#define E_ARGS 0x0200
#define E_RENDER 0x0300
#define E_INIT 0x0400
#define E_EVENT 0x0500

extern const char *prog_name;

#define NBUFS 10
struct buffer_t {
    char *filename;
    char **lines;
    int64_t clines;
};
extern struct buffer_t *buffers[NBUFS];
extern int buf_idx;

extern int rows, cols;

extern int redraw_display_needed, redraw_bar_needed;
extern int running;

extern int drow, dcol, row, col;

void usage(FILE *out);
void help();
char *process_args(int argc, char *const *argv);

int open_buffer(const char *name);
void close_buffer(int idx);
void close_all_buffers();

void init();
void cleanup();

void redraw(int force);

int handle_events();
