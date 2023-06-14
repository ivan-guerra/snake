#include "game/game.hpp"

#include <algorithm>
#include <iostream>
#include <random>

namespace snake {
namespace game {

void SnakeGame::SpawnSnake() {
    /* create a random shuffle of the possible directions the snake can go */
    std::vector<Direction> directions = {Direction::kUp, Direction::kDown,
                                         Direction::kLeft, Direction::kRight};
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
    std::shuffle(directions.begin(), directions.end(), rng);

    /* spawn the snake head in the center of the screen with a random direction
     */
    snake_.push_back({.row = screen_dim_.height / 2,
                      .col = screen_dim_.width / 2,
                      .direction = directions[0]});
}

void SnakeGame::MoveSnake(const Direction& new_direction) {
    /* shift all but the head tiles into their predecessor's position */
    Snake tmp = snake_;
    for (std::size_t i = 1; i < snake_.size(); ++i) {
        snake_[i] = tmp[i - 1];
    }

    /* walk the head forward in whatever direction it's facing */
    Tile& head = snake_.front();
    head.direction = new_direction;
    switch (snake_.front().direction) {
        case Direction::kUp:
            head.row--;
            break;
        case Direction::kDown:
            head.row++;
            break;
        case Direction::kLeft:
            head.col--;
            break;
        case Direction::kRight:
            head.col++;
            break;
        case Direction::kNone:
            break;
    }
}

void SnakeGame::ExtendSnake() {
    Tile new_snake_tile = snake_.back();

    /* the new tile's location is the current snake tail's location shifted
     * opposite the snake tail's direction */
    switch (new_snake_tile.direction) {
        case Direction::kUp:
            new_snake_tile.row++;
            break;
        case Direction::kDown:
            new_snake_tile.row--;
            break;
        case Direction::kLeft:
            new_snake_tile.col++;
            break;
        case Direction::kRight:
            new_snake_tile.col--;
            break;
        case Direction::kNone:
            break;
    }
    snake_.push_back(new_snake_tile);
}

bool SnakeGame::IsGameOver() const {
    /* check if the snake overlaps itself at any tile */
    for (std::size_t i = 0; i < snake_.size(); ++i) {
        for (std::size_t j = i + 1; j < snake_.size(); ++j) {
            if (snake_[i] == snake_[j]) {
                return true;
            }
        }
    }

    /* verify the head snake tile is in bounds */
    bool is_in_row_bounds =
        (snake_.front().row >= border_) &&
        (snake_.front().row < (screen_dim_.height - border_));
    bool is_in_col_bounds =
        (snake_.front().col >= border_) &&
        (snake_.front().col < (screen_dim_.width - border_));

    return (!is_in_row_bounds || !is_in_col_bounds);
}

bool SnakeGame::SnakeWins() const {
    /* check whether the snake is occupying every possible target location */
    for (const Tile& target_tile : targets_) {
        bool found = false;
        for (const Tile& snake_tile : snake_) {
            if (snake_tile == target_tile) {
                found = true;
                break;
            }
        }
        if (!found) { /* looks like there's at least one open target location */
            return false;
        }
    }
    return true;
}

SnakeGame::SnakeGame(const ScreenDimension& dim, int border)
    : game_over_(false),
      score_(0),
      border_(border),
      screen_dim_(dim),
      curr_target_(0) {
    Reset();
}

void SnakeGame::Tick(const Direction& new_direction) {
    MoveSnake(new_direction);

    if (IsGameOver()) { /* do nothing if the game has already ended */
        game_over_ = true;
        return;
    }

    /* looks like the snake ate its target */
    if (snake_[0] == targets_[curr_target_]) {
        score_ += kScoreIncrement;

        ExtendSnake();

        if (SnakeWins()) {
            game_over_ = true;
            return;
        }

        /* search for the next target tile that is not occupied by the snake */
        while (std::find(snake_.begin(), snake_.end(),
                         targets_[curr_target_]) != snake_.end()) {
            curr_target_ =
                (curr_target_ + 1) % static_cast<int>(targets_.size());
        }
    }
}

void SnakeGame::Reset() {
    game_over_ = false;
    score_ = 0;

    /* generate a randomly shuffled vector of potential target locations */
    targets_.clear();
    for (int i = border_; i < (screen_dim_.height - border_); ++i) {
        for (int j = border_; j < (screen_dim_.width - border_); ++j) {
            targets_.push_back(
                {.row = i, .col = j, .direction = Direction::kNone});
        }
    }
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
    std::shuffle(targets_.begin(), targets_.end(), rng);
    curr_target_ = 0;

    /* respawn the snake */
    snake_.clear();
    SpawnSnake();

    /* ensure the target does not overlap the snake head */
    while (targets_[curr_target_] == snake_.front()) {
        curr_target_ = (curr_target_ + 1) % static_cast<int>(targets_.size());
    }
}

}  // namespace game
}  // namespace snake
