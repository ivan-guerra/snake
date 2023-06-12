#include "graphics/screen.hpp"

#include <ncurses.h>

#include <string>
#include <vector>

namespace snake {
namespace graphics {

static void DrawTarget(const snake::game::SnakeGame& game) {
    snake::game::Tile target = game.GetTargetTile();

    attron(A_BOLD);
    attron(COLOR_PAIR(Color::kRed));
    mvaddch(target.row, target.col, ACS_DIAMOND);
    attroff(COLOR_PAIR(Color::kRed));
    attroff(A_BOLD);
}

static void DrawSnake(const snake::game::SnakeGame& game) {
    snake::game::Snake snake = game.GetSnake();

    attron(A_BOLD);
    attron(COLOR_PAIR(Color::kGreen));
    const auto& head = snake.front();
    switch (head.direction) {
        case snake::game::Direction::kUp:
            mvaddch(head.row, head.col, '^');
            break;
        case snake::game::Direction::kDown:
            mvaddch(head.row, head.col, 'v');
            break;
        case snake::game::Direction::kLeft:
            mvaddch(head.row, head.col, '<');
            break;
        case snake::game::Direction::kRight:
            mvaddch(head.row, head.col, '>');
            break;
        case snake::game::Direction::kNone:
            mvaddch(head.row, head.col, '?');
            break;
    }
    attroff(A_BOLD);

    for (std::size_t i = 1; i < snake.size(); ++i) {
        mvaddch(snake[i].row, snake[i].col, '+');
    }
    attroff(COLOR_PAIR(Color::kGreen));
}

snake::game::ScreenDimension InitScreen() {
    initscr();
    cbreak();             /* disable line buffering */
    keypad(stdscr, TRUE); /* enable the keypad so we can work with arrow keys */
    noecho();             /* disable character echoing */
    curs_set(0);          /* hide the cursor */

    /* initialize all game colors */
    start_color();
    init_pair(Color::kRed, COLOR_RED, COLOR_BLACK);
    init_pair(Color::kGreen, COLOR_GREEN, COLOR_BLACK);
    init_pair(Color::kCyan, COLOR_CYAN, COLOR_BLACK);

    /* fetch the screen dimensions */
    snake::game::ScreenDimension screen_dim = {.width = 0, .height = 0};
    getmaxyx(stdscr, screen_dim.height, screen_dim.width);

    return screen_dim;
}

void TerminateScreen() { endwin(); }

void EnableInputDelay(int delay_ms) { timeout(delay_ms); }

void DisableInputDelay() { timeout(-1); }

snake::game::Direction ReadKeypad() {
    int key = getch();
    switch (key) {
        case KEY_UP:
            return snake::game::Direction::kUp;
            break;
        case KEY_DOWN:
            return snake::game::Direction::kDown;
            break;
        case KEY_LEFT:
            return snake::game::Direction::kLeft;
            break;
        case KEY_RIGHT:
            return snake::game::Direction::kRight;
            break;
        default:
            return snake::game::Direction::kNone;
    }
}

void DrawSnakeScreen(const snake::game::SnakeGame& game) {
    clear();
    DrawTarget(game);
    DrawSnake(game);
    refresh();
}

void DrawGameOverScreen(const snake::game::SnakeGame& game) {
    clear();

    snake::game::ScreenDimension dim = game.GetScreenDimension();

    /* display the game over banner */
    const std::vector<std::string> kGameOverBanner = {
        " _____   ___  ___  ___ _____ ",
        "|  __ \\ / _ \\ |  \\/  ||  ___|",
        "| |  \\// /_\\ \\| .  . || |__  ",
        "| | __ |  _  || |\\/| ||  __| ",
        "| |_\\ \\| | | || |  | || |___ ",
        " \\____/\\_| |_/\\_|  |_/\\____/ ",
        " _____  _   _  _____ ______  ",
        "|  _  || | | ||  ___|| ___ \\ ",
        "| | | || | | || |__  | |_/ / ",
        "| | | || | | ||  __| |    /  ",
        "\\ \\_/ /\\ \\_/ /| |___ | |\\ \\  ",
        " \\___/  \\___/ \\____/ \\_| \\_| ",
    };
    attron(COLOR_PAIR(Color::kGreen));
    for (std::size_t i = 0; i < kGameOverBanner.size(); ++i) {
        mvprintw(static_cast<int>(i),
                 (dim.width - static_cast<int>(kGameOverBanner[i].size())) / 2,
                 "%s\n", kGameOverBanner[i].c_str());
    }
    attroff(COLOR_PAIR(Color::kGreen));

    /* display the player's score */
    attron(COLOR_PAIR(Color::kCyan));
    const std::string kScoreBanner("SCORE");
    mvprintw(static_cast<int>(kGameOverBanner.size()) + 2,
             (dim.width - static_cast<int>(kScoreBanner.size())) / 2,
             "%s: %d\n", kScoreBanner.c_str(), game.GetScore());
    attroff(COLOR_PAIR(Color::kCyan));

    /* display the quit banner */
    const std::string kQuitBanner("press q to quit");
    mvprintw(dim.height - 1, 0, "%s", kQuitBanner.c_str());

    /* wait for the user to enter 'q' before quitting */
    int c = 0;
    while ((c = getch()) != 'q') {
    }
}

}  // namespace graphics
}  // namespace snake
