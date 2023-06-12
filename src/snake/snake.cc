#include "game/game.hpp"
#include "graphics/screen.hpp"

void RunGameLoop(snake::game::SnakeGame& game) {
    /* setup to timeout on input after kReadDelayMs milliseconds */
    const int kReadDelayMs = 100;
    snake::graphics::EnableInputDelay(kReadDelayMs);

    snake::game::Direction curr_direction = game.GetSnake().front().direction;
    while (!game.GameOver()) {
        snake::game::Direction new_direction = snake::graphics::ReadKeypad();

        /* update the direction only if the user provided one */
        if (new_direction != snake::game::Direction::kNone) {
            curr_direction = new_direction;
        }
        game.Tick(curr_direction);
        snake::graphics::DrawSnakeScreen(game);
    }
    snake::graphics::DisableInputDelay();
}

int main() {
    /* configure the screen */
    snake::game::ScreenDimension screen_dim = snake::graphics::InitScreen();

    /* draw the initial game screen */
    snake::game::SnakeGame game(screen_dim);
    snake::graphics::DrawSnakeScreen(game);

    RunGameLoop(game);

    /* show the game over screen with the score and exit */
    snake::graphics::DrawGameOverScreen(game);
    snake::graphics::TerminateScreen();

    return 0;
}
