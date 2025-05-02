#include "inspect.h"

#include <string.h>

#include <ncurses.h>

#define CTRL(key) ((key) & 0x1f)

int clamp(int n, int min, int max) {
    const int t = n < min ? min : n;
    return t > max ? max : t;
}

void move_cursor(int dr, int dc) {
    row = clamp(row + dr, 0, clines - 1);
    col = clamp(col + dc, 0, strlen(lines[row]));
    redraw_display_needed = TRUE;
    if (row >= drow + rows - 3) drow = row - rows + 3 + 1;
    else if (row < drow) drow = row;
    else redraw_display_needed = FALSE;
}

void handle_events() {
    int c = getch();
    switch (c) {
    case 'q': running = FALSE; break;
    case 'h':
        move_cursor(0, -1);
        redraw_bar_needed = TRUE;
        break;
    case 'j':
        move_cursor(1, 0);
        redraw_bar_needed = TRUE;
        break;
    case 'k':
        move_cursor(-1, 0);
        redraw_bar_needed = TRUE;
        break;
    case 'l':
        move_cursor(0, 1);
        redraw_bar_needed = TRUE;
        break;
    case CTRL('d'):
        move_cursor(rows / 2, 0);
        redraw_bar_needed = TRUE;
        break;
    case CTRL('u'):
        move_cursor(-rows / 2, 0);
        redraw_bar_needed = TRUE;
        break;
    case CTRL('f'):
        move_cursor(rows, 0);
        redraw_bar_needed = TRUE;
        break;
    case CTRL('b'):
        move_cursor(-rows, 0);
        redraw_bar_needed = TRUE;
        break;
    default: break;
    }
}
