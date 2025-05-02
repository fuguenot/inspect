#include "inspect.h"

#include <math.h>
#include <string.h>

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
        if (row + drow < clines - 1) {
            row++;
            col = fminf(col, strlen(lines[row + drow]));
        }
        redraw_bar_needed = TRUE;
        break;
    case 'k':
        if (row > 0) {
            row--;
            col = fminf(col, strlen(lines[row + drow]));
        }
        redraw_bar_needed = TRUE;
        break;
    case 'l':
        if (col < strlen(lines[row + drow])) col++;
        redraw_bar_needed = TRUE;
        break;
    default: break;
    }
}
