#include "inspect.h"

#include <stdlib.h>

#include <ncurses.h>

WINDOW *display;
WINDOW *bar;

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
    redraw_bar_needed = FALSE;
    drow = 0;
    dcol = 0;
    row = 0;
    col = 0;

    refresh();
}

void cleanup() {
    for (int i = 0; i < clines; i++)
        free(lines[i]);
    free(lines);
    wborder(display, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(display);
    delwin(display);
    wborder(bar, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(bar);
    delwin(bar);
    endwin();
}

void redraw_display() {
    wclear(display);
    wrefresh(display);
    for (int i = 0; i < rows; i++)
        if (i + drow < clines) mvwprintw(display, i, 0, "%s", lines[i + drow]);
    wrefresh(display);
}

void redraw_bar() {
    wclear(bar);
    wrefresh(bar);
    mvwprintw(bar, 0, 0, "%s | %d:%d", file, row, col);
    mvwprintw(bar, 1, 0, "inspect v0.4.0 | ? for keybinds");
    wrefresh(bar);
}

void redraw(int force) {
    if (force || redraw_display_needed) redraw_display();
    if (force || redraw_bar_needed) redraw_bar();
    move(row, col);
}
