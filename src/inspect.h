#pragma once

#include <stdio.h>

extern const char *prog_name;
extern const char *file;
extern char *buf;

extern int rows, cols;

extern int redraw_display_needed, redraw_bar_needed;
extern int running;

extern int drow, dcol, row, col;

void usage(FILE *out);
void help();

void process_args(int argc, char *const *argv);
void load_file();

void init();
void cleanup();

void redraw(int force);

void handle_events();
