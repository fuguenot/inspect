#include "inspect.h"

#include <string.h>

#include <ncurses.h>

#include "error.h"

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
        if (find_next_buffer(true, true))
            redraw_display_needed = true;
        else
            running = false;
        break;
    case '?':
        buf_idx = open_buffer("inspect-help", true, help_path);
        if (buf_idx == RET_ERR) return RET_ERR;
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
        if (find_next_buffer(true, false)) redraw_display_needed = true;
        break;
    case KEY_RIGHT:
        if (find_next_buffer(false, true)) redraw_display_needed = true;
        break;
    case 'X':
        if (bufs[buf_idx]->readonly) {
            error.code = E_READONLY;
            return RET_ERR;
        } else
            remove(bufs[buf_idx]->filename);
        close_buffer(buf_idx);
        if (find_next_buffer(true, true))
            redraw_display_needed = true;
        else
            running = false;
        break;
    case '\r':
    case '\n':
        error.code = E_OK;
        error.details = NULL;
        break;
    default: break;
    }
    return 0;
}
