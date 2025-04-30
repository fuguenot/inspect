#include <ncurses.h>

int main() {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);

    int c;
    int running = TRUE;
    while (running) {
        c = getch();
        switch (c) {
        case 'q': running = false; break;
        }
    }

    endwin();
}
