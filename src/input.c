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
    case 'Q': running = FALSE; break;
    case 'q':
        close_buffer(buf_idx);
        for (int i = buf_idx - 1; i >= 0; i--)
            if (buffers[i] != NULL) {
                buf_idx = i;
                redraw_display_needed = TRUE;
                break;
            }
        if (buffers[buf_idx] == NULL)
            for (int i = buf_idx + 1; i < NBUFS; i++)
                if (buffers[i] != NULL) {
                    buf_idx = i;
                    redraw_display_needed = TRUE;
                    break;
                }
        if (buffers[buf_idx] == NULL) running = FALSE;
        break;
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
        if (buffers[NBUFS - 1] != NULL) {
            buf_idx = NBUFS - 1;
            redraw_display_needed = TRUE;
        }
        break;
    case '1' ... '9':
        if (buffers[c - '0' - 1] != NULL) {
            buf_idx = c - '0' - 1;
            redraw_display_needed = TRUE;
        }
        break;
    case KEY_LEFT:
        for (int i = buf_idx - 1; i >= 0; i--) {
            if (buffers[i] != NULL) {
                buf_idx = i;
                redraw_display_needed = TRUE;
                break;
            }
        }
        break;
    case KEY_RIGHT:
        for (int i = buf_idx + 1; i < NBUFS; i++) {
            if (buffers[i] != NULL) {
                buf_idx = i;
                redraw_display_needed = TRUE;
                break;
            }
        }
        break;
    default: break;
    }
    return 0;
}
