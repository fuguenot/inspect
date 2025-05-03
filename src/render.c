#include "inspect.h"

#include <locale.h>
#include <stdlib.h>

#include <ncurses.h>

#include "error.h"

WINDOW *display;
WINDOW *bar;

#define BAR_SPACE 3

enum color_pair_e : int16_t {
    BAR_PAIR = 1,
    SEL_PAIR,
    FILE_PAIR,
    ERR_PAIR
};

void init_ui() {
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    nonl();
    keypad(stdscr, true);

    if (has_colors()) {
        start_color();
        init_pair(BAR_PAIR, COLOR_WHITE, COLOR_BLACK);
        init_pair(SEL_PAIR, COLOR_GREEN, COLOR_BLACK);
        init_pair(FILE_PAIR, COLOR_BLUE, COLOR_BLACK);
        init_pair(ERR_PAIR, COLOR_WHITE, COLOR_RED);
    }

    getmaxyx(stdscr, rows, cols);

    display = newwin(rows - BAR_SPACE, cols, 0, 0);
    bar = newwin(BAR_SPACE, cols, rows - BAR_SPACE, 0);

    running = true;
    redraw_display_needed = false;
    redraw_bar_needed = false;

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
        if (i + bufs[buf_idx]->drow < bufs[buf_idx]->nlines)
            mvwaddstr(display,
                      i,
                      0,
                      bufs[buf_idx]->lines[i + bufs[buf_idx]->drow]);
    wrefresh(display);
}

void redraw_bar() {
    wclear(bar);
    wrefresh(bar);
    wattron(bar, A_BOLD | COLOR_PAIR(BAR_PAIR));
    mvwaddstr(bar, 0, 0, "inspect v0.4.0 |");
    for (int i = 0; i < NBUFS; i++) {
        waddch(bar, ' ');
        if (i == buf_idx)
            wattron(bar, COLOR_PAIR(SEL_PAIR));
        else if (bufs[i] == NULL)
            wattron(bar, A_DIM);
        wprintw(bar, "[%d]", (i + 1) % 10);
        if (i == buf_idx)
            wattroff(bar, COLOR_PAIR(SEL_PAIR));
        else if (bufs[i] == NULL)
            wattroff(bar, A_DIM);
        wattron(bar, COLOR_PAIR(BAR_PAIR));
        waddch(bar, ' ');
    }
    wattroff(bar, A_BOLD | COLOR_PAIR(BAR_PAIR));
    wattron(bar, A_BOLD | COLOR_PAIR(FILE_PAIR));
    mvwprintw(bar, 1, 0, bufs[buf_idx]->filename);
    wattroff(bar, A_BOLD | COLOR_PAIR(FILE_PAIR));
    wprintw(bar, " | %d:%d", bufs[buf_idx]->row, bufs[buf_idx]->col);
    if (error.code != E_OK) {
        wattron(bar, COLOR_PAIR(ERR_PAIR));
        mvwprintw(bar, 2, 0, "%s", get_errmsg());
        wattroff(bar, COLOR_PAIR(ERR_PAIR));
    }
    wrefresh(bar);
}

void redraw(bool force) {
    if (force || redraw_display_needed) redraw_display();
    if (force || redraw_bar_needed) redraw_bar();
    move(bufs[buf_idx]->row - bufs[buf_idx]->drow,
         bufs[buf_idx]->col - bufs[buf_idx]->dcol);
}
