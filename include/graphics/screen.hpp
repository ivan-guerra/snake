#ifndef SCREEN_HPP_
#define SCREEN_HPP_

#include "game/game.hpp"

namespace snake {
namespace graphics {

enum Color {
    kRed = 1,
    kGreen = 2,
    kCyan = 3,
};

snake::game::ScreenDimension InitScreen();
void TerminateScreen();

void EnableInputDelay(int delay_ms);
void DisableInputDelay();

snake::game::Direction ReadKeypad();

void DrawSnakeScreen(const snake::game::SnakeGame& game);
void DrawGameOverScreen(const snake::game::SnakeGame& game);

}  // namespace graphics
}  // namespace snake

#endif
