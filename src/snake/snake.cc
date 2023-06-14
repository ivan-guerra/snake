#include "game/game.hpp"
#include "graphics/screen.hpp"

void RunGameLoop(snake::game::SnakeGame& game,
                 const snake::graphics::GameMode& mode) {
    /* adjust the input delay in order tick the game faster or slower */
    const int kEasyModeDelayMs = 150;
    const int kMedModeDelayMs = 100;
    const int kHardModeDelayMs = 75;
    switch (mode) {
        case snake::graphics::GameMode::kEasy:
            snake::graphics::EnableInputDelay(kEasyModeDelayMs);
            break;
        case snake::graphics::GameMode::kMedium:
            snake::graphics::EnableInputDelay(kMedModeDelayMs);
            break;
        case snake::graphics::GameMode::kHard:
            snake::graphics::EnableInputDelay(kHardModeDelayMs);
            break;
    }

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

    /* display the start menu and fetch the user's game mode selection */
    snake::graphics::GameMode mode = snake::graphics::PromptForGameMode();

    /* draw the initial game screen */
    snake::game::SnakeGame game(screen_dim);
    snake::graphics::DrawSnakeScreen(game);

    RunGameLoop(game, mode);

    /* show the game over screen with the score and exit */
    snake::graphics::DrawGameOverScreen(game);
    snake::graphics::TerminateScreen();

    return 0;
}
