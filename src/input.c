#include "inspect.h"

#include <string.h>

#include <ncurses.h>

#define CTRL(key) ((key) & 0x1f)

int clamp(int n, int min, int max) {
    const int t = n < min ? min : n;
    return t > max ? max : t;
}

void move_cursor(int dr, int dc) {
    row = clamp(row + dr, 0, buffers[buf_idx]->clines - 1);
    col = clamp(col + dc, 0, strlen(buffers[buf_idx]->lines[row]));
    redraw_display_needed = TRUE;
    if (row >= drow + rows - 3)
        drow = row - rows + 3 + 1;
    else if (row < drow)
        drow = row;
    else
        redraw_display_needed = FALSE;
}

int handle_events() {
    int c = getch();
    redraw_bar_needed = TRUE;
    switch (c) {
    case 'q': running = FALSE; break;
    case '?':
        buf_idx = open_buffer("inspect-help");
        if (buf_idx < 0) return buf_idx;
        redraw_display_needed = TRUE;
        break;
    case 'h': move_cursor(0, -1); break;
    case 'j': move_cursor(1, 0); break;
    case 'k': move_cursor(-1, 0); break;
    case 'l': move_cursor(0, 1); break;
    case CTRL('d'): move_cursor(rows / 2, 0); break;
    case CTRL('u'): move_cursor(-rows / 2, 0); break;
    case CTRL('f'): move_cursor(rows, 0); break;
    case CTRL('b'): move_cursor(-rows, 0); break;
    case '0':
        if (buffers[0] != NULL) buf_idx = 0;
        redraw_display_needed = TRUE;
        break;
    case '1':
        if (buffers[1] != NULL) buf_idx = 1;
        redraw_display_needed = TRUE;
        break;
    case '2':
        if (buffers[2] != NULL) buf_idx = 2;
        redraw_display_needed = TRUE;
        break;
    case '3':
        if (buffers[3] != NULL) buf_idx = 3;
        redraw_display_needed = TRUE;
        break;
    case '4':
        if (buffers[4] != NULL) buf_idx = 4;
        redraw_display_needed = TRUE;
        break;
    case '5':
        if (buffers[5] != NULL) buf_idx = 5;
        redraw_display_needed = TRUE;
        break;
    case '6':
        if (buffers[6] != NULL) buf_idx = 6;
        redraw_display_needed = TRUE;
        break;
    case '7':
        if (buffers[7] != NULL) buf_idx = 7;
        redraw_display_needed = TRUE;
        break;
    case '8':
        if (buffers[8] != NULL) buf_idx = 8;
        redraw_display_needed = TRUE;
        break;
    case '9':
        if (buffers[9] != NULL) buf_idx = 9;
        redraw_display_needed = TRUE;
        break;
    default: break;
    }
    return 0;
}
