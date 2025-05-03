#include "inspect.h"

#include <string.h>

#include <ncurses.h>

#define CTRL(key) ((key) & 0x1f)

int clamp(int n, int min, int max) {
    const int t = n < min ? min : n;
    return t > max ? max : t;
}

void move_cursor(int dr, int dc) {
    bufs[buf_idx]->row =
        clamp(bufs[buf_idx]->row + dr, 0, bufs[buf_idx]->nlines - 1);
    bufs[buf_idx]->col =
        clamp(bufs[buf_idx]->col + dc,
              0,
              strlen(bufs[buf_idx]->lines[bufs[buf_idx]->row]));
    redraw_display_needed = true;
    if (bufs[buf_idx]->row >= bufs[buf_idx]->drow + rows - 3)
        bufs[buf_idx]->drow = bufs[buf_idx]->row - rows + 3 + 1;
    else if (bufs[buf_idx]->row < bufs[buf_idx]->drow)
        bufs[buf_idx]->drow = bufs[buf_idx]->row;
    else
        redraw_display_needed = false;
}

int handle_events() {
    int c = getch();
    redraw_bar_needed = true;
    switch (c) {
    case 'Q': running = false; break;
    case 'q':
        close_buffer(buf_idx);
        for (int i = buf_idx - 1; i >= 0; i--)
            if (bufs[i] != NULL) {
                buf_idx = i;
                redraw_display_needed = true;
                break;
            }
        if (bufs[buf_idx] == NULL)
            for (int i = buf_idx + 1; i < NBUFS; i++)
                if (bufs[i] != NULL) {
                    buf_idx = i;
                    redraw_display_needed = true;
                    break;
                }
        if (bufs[buf_idx] == NULL) running = false;
        break;
    case '?':
        buf_idx = open_buffer("inspect-help");
        if (buf_idx < 0) return buf_idx;
        redraw_display_needed = true;
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
        if (bufs[NBUFS - 1] != NULL) {
            buf_idx = NBUFS - 1;
            redraw_display_needed = true;
        }
        break;
    case '1' ... '9':
        if (bufs[c - '0' - 1] != NULL) {
            buf_idx = c - '0' - 1;
            redraw_display_needed = true;
        }
        break;
    case KEY_LEFT:
        for (int i = buf_idx - 1; i >= 0; i--) {
            if (bufs[i] != NULL) {
                buf_idx = i;
                redraw_display_needed = true;
                break;
            }
        }
        break;
    case KEY_RIGHT:
        for (int i = buf_idx + 1; i < NBUFS; i++) {
            if (bufs[i] != NULL) {
                buf_idx = i;
                redraw_display_needed = true;
                break;
            }
        }
        break;
    default: break;
    }
    return 0;
}
