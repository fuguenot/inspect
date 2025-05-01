#include <stdlib.h>
#include <unistd.h>

#include <ncurses.h>

const char *prog_name;

const char *file;
FILE *fp;

int rows, cols;

WINDOW *display;
WINDOW *bar;
int redraw_display_needed, redraw_bar_needed;

int running;

int drow, dcol;
int row, col;
int c;

#define USAGE_STR "usage: %s [-h] <file>\n"
#define HELP_STR                                  \
    "inspect: simple tool for looking at files\n" \
    "options:\n"                                  \
    "  -h : prints this help string\n"            \
    "  file : the file to look at\n"

void usage(FILE *out) {
    fprintf(out, USAGE_STR, prog_name);
}

void help() {
    printf(HELP_STR);
}

void process_args(int argc, char *const *argv) {
    prog_name = argv[0];
    int o;
    while ((o = getopt(argc, argv, "h")) != -1) {
        switch (o) {
        case 'h':
            usage(stdout);
            help();
            exit(0);
            break;
        default:
            usage(stderr);
            exit(1);
            break;
        }
    }
    if (optind >= argc) {
        usage(stderr);
        exit(1);
    }
    file = argv[optind];
}

void load_file() {
    fp = fopen(file, "r");
    if (fp == NULL) {
        perror(prog_name);
        fprintf(stderr, "%s: failed to open file '%s'\n", prog_name, file);
        exit(1);
    }
}

void init() {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);

    getmaxyx(stdscr, rows, cols);

    display = newwin(rows - 3, cols, 0, 0);
    bar = newwin(3, cols, rows - 3, 0);

    running = TRUE;
    redraw_display_needed = FALSE;
    drow = 0;
    dcol = 0;
    row = 0;
    col = 0;

    refresh();
}

void redraw_display() {
    wclear(display);
    wrefresh(display);
    wborder(display, '|', '|', '-', '-', '+', '+', '+', '+');
    wprintw(display, "asdfkjhabsfkhbsdf");
    wrefresh(display);
}

void redraw_bar() {
    wclear(bar);
    wrefresh(bar);
    mvwprintw(bar, 0, 0, "%s | %d:%d", file, row, col);
    mvwprintw(bar, 1, 0, "inspect v0.4.0 | ? for keybinds");
    wrefresh(bar);
}

void redraw() {
    redraw_display();
    redraw_bar();
}

void cleanup() {
    wborder(display, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(display);
    delwin(display);
    wborder(bar, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(bar);
    delwin(bar);
    endwin();
}

int main(int argc, char *const *argv) {
    process_args(argc, argv);
    load_file();
    init();
    redraw();
    while (running) {
        move(row, col);
        if (redraw_display_needed) redraw_display();
        if (redraw_bar_needed) redraw_bar();
        c = getch();
        switch (c) {
        case 'q': running = FALSE; break;
        case 'h':
            if (col > 0) col--;
            redraw_bar_needed = TRUE;
            break;
        case 'j':
            row++;
            redraw_bar_needed = TRUE;
            break;
        case 'k':
            if (row > 0) row--;
            redraw_bar_needed = TRUE;
            break;
        case 'l':
            col++;
            redraw_bar_needed = TRUE;
            break;
        default: break;
        }
    }

    cleanup();
}
