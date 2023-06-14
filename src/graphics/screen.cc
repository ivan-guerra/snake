#include "graphics/screen.hpp"

#include <menu.h>
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

GameMode PromptForGameMode() {
    clear();

    const std::vector<std::string> kTitleBanner = {
        " _____  _   _   ___   _   __ _____ ",
        "/  ___|| \\ | | / _ \\ | | / /|  ___|",
        "\\ `--. |  \\| |/ /_\\ \\| |/ / | |__  ",
        " `--. \\| . ` ||  _  ||    \\ |  __| ",
        "/\\__/ /| |\\  || | | || |\\  \\| |___ ",
        "\\____/ \\_| \\_/\\_| |_/\\_| \\_/\\____/ ",

    };

    /* display the title banner */
    int row = 0;
    int col = 0;
    getmaxyx(stdscr, row, col);
    (void)row; /* avoid warning regarding unused row variable */
    attron(A_BOLD);
    for (std::size_t i = 0; i < kTitleBanner.size(); ++i) {
        if (i & 1) {
            attron(COLOR_PAIR(Color::kRed));
        } else {
            attron(COLOR_PAIR(Color::kGreen));
        }

        mvprintw(static_cast<int>(i) + 1,
                 (col - static_cast<int>(kTitleBanner[i].size())) / 2, "%s\n",
                 kTitleBanner[i].c_str());

        if (i & 1) {
            attroff(COLOR_PAIR(Color::kRed));
        } else {
            attroff(COLOR_PAIR(Color::kGreen));
        }
    }
    attroff(A_BOLD);

    /* display the mode prompt */
    attron(COLOR_PAIR(Color::kCyan) | A_BOLD);
    std::string mode_prompt("Choose your difficulty:");
    mvprintw(static_cast<int>(kTitleBanner.size()) + 2,
             (col - static_cast<int>(mode_prompt.size())) / 2, "%s",
             mode_prompt.c_str());
    attroff(COLOR_PAIR(Color::kCyan) | A_BOLD);

    const std::vector<std::string> kModes = {
        "easy",
        "medium",
        "hard",
    };

    /* create menu items */
    std::vector<ITEM*> mode_items(kModes.size() + 1, nullptr);
    for (std::size_t i = 0; i < kModes.size(); ++i) {
        mode_items[i] = new_item(kModes[i].c_str(), "");
    }

    /* create the start menu */
    MENU* start_menu = new_menu(mode_items.data());
    menu_opts_off(start_menu, O_SHOWDESC);
    const int kNumMenuLines = 3;
    const int kNumMenuCols = 1;
    set_menu_format(start_menu, kNumMenuLines, kNumMenuCols);
    set_menu_mark(start_menu, "");

    /* create the window to be associated with the menu */
    const int kNumLines = 10;
    const int kNumCols = 50;
    const int kColOffset = 7;
    WINDOW* start_menu_win =
        newwin(kNumLines, kNumCols, static_cast<int>(kTitleBanner.size()) + 3,
               (col - kColOffset) / 2);
    keypad(start_menu_win, TRUE);

    /* set main window and sub window */
    const int kSubmenuNumLines = 3;
    const int kSubmenuNumCols = 20;
    const int kSubmenuRow = 0;
    const int kSubmenuCol = 0;
    set_menu_win(start_menu, start_menu_win);
    set_menu_sub(start_menu, derwin(start_menu_win, kSubmenuNumLines,
                                    kSubmenuNumCols, kSubmenuRow, kSubmenuCol));

    refresh(); /* display the title and mode prompt */

    /* post and display the menu */
    post_menu(start_menu);
    wrefresh(start_menu_win);

    /* allow the user to cycle through the menu until they make a selection with
     * the ENTER key */
    const int kAsciiEnter = 10;
    int c = 0;
    while ((c = wgetch(start_menu_win)) != kAsciiEnter) {
        switch (c) {
            case KEY_DOWN:
                menu_driver(start_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(start_menu, REQ_UP_ITEM);
                break;
        }
        wrefresh(start_menu_win);
    }

    /* determine the game mode based on the user's menu selection */
    std::string mode(item_name(current_item(start_menu)));
    GameMode ret = GameMode::kEasy;
    if (kModes[0] == mode) {
        ret = GameMode::kEasy;
    } else if (kModes[1] == mode) {
        ret = GameMode::kMedium;
    } else {
        ret = GameMode::kHard;
    }

    /* free all resources */
    unpost_menu(start_menu);
    free_menu(start_menu);
    for (std::size_t i = 0; i < kModes.size(); ++i) {
        free_item(mode_items[i]);
    }
    return ret;
}

void DrawSnakeScreen(const snake::game::SnakeGame& game) {
    clear();

    if (game.GetBorder()) {
        box(stdscr, 0, 0);
    }
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
