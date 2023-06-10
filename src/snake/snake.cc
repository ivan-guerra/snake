#include <ncurses.h>

#include <string>

int main() {
    initscr();

    int row = 0;
    int col = 0;
    getmaxyx(stdscr, row, col);

    std::string mesg("say hello to the snake");
    mvprintw(row / 2, (col - static_cast<int>(mesg.size())) / 2, "%s",
             mesg.c_str());

    refresh();
    getch();
    endwin();

    return 0;
}
