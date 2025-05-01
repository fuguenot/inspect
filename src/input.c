#include "inspect.h"

#include <ncurses.h>

void handle_events() {
    int c = getch();
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
