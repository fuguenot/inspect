#include "inspect.h"

#include <stdlib.h>

#include <ncurses.h>

WINDOW *display;
WINDOW *bar;

#define BAR_PAIR 1
#define SEL_PAIR 2

void init() {
    initscr();
    cbreak();
    noecho();
    nonl();
    keypad(stdscr, TRUE);

    if (has_colors() == TRUE) start_color();
    init_pair(BAR_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(SEL_PAIR, COLOR_GREEN, COLOR_BLACK);

    getmaxyx(stdscr, rows, cols);

    display = newwin(rows - 3, cols, 0, 0);
    bar = newwin(3, cols, rows - 3, 0);

    running = TRUE;
    redraw_display_needed = FALSE;
    redraw_bar_needed = FALSE;
    drow = 0;
    dcol = 0;
    row = 0;
    col = 0;

    refresh();
}

void cleanup() {
    delwin(display);
    delwin(bar);
    endwin();
}

void redraw_display() {
    wclear(display);
    wrefresh(display);
    for (int i = 0; i < rows; i++)
        if (i + drow < buffers[buf_idx]->clines)
            mvwaddstr(display, i, 0, buffers[buf_idx]->lines[i + drow]);
    wrefresh(display);
}

void redraw_bar() {
    wclear(bar);
    wrefresh(bar);
    wattron(bar, A_BOLD | COLOR_PAIR(BAR_PAIR));
    mvwaddstr(bar, 0, 0, "Buffer ");
    for (int i = 0; i < NBUFS; i++) {
        if (i == buf_idx) wattron(bar, COLOR_PAIR(SEL_PAIR));
        else if (buffers[i] == NULL) wattron(bar, A_DIM);
        wprintw(bar, "[%d]", i);
        if (i == buf_idx) wattroff(bar, COLOR_PAIR(SEL_PAIR));
        else if (buffers[i] == NULL) wattroff(bar, A_DIM);
        wattron(bar, COLOR_PAIR(BAR_PAIR));
        waddch(bar, ' ');
    }
    wattroff(bar, A_BOLD | COLOR_PAIR(BAR_PAIR));
    mvwprintw(bar, 1, 0, "%s | %d:%d", buffers[buf_idx]->filename, row, col);
    mvwaddstr(bar, 2, 0, "inspect v0.4.0 | ? for keybinds");
    wrefresh(bar);
}

void redraw(int force) {
    if (force || redraw_display_needed) redraw_display();
    if (force || redraw_bar_needed) redraw_bar();
    move(row - drow, col - dcol);
}
