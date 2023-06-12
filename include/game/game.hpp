#ifndef GAME_
#define GAME_

#include <vector>

namespace snake {
namespace game {

enum class Direction {
    kUp,
    kDown,
    kLeft,
    kRight,
    kNone,
};

struct ScreenDimension {
    int width = 0;
    int height = 0;
};

/**
 * Representation of a single location on the screen.
 *
 * The screen is divided up into a grid of 2D coordinate locations called Tiles.
 * Tiles can optionally have a direction when they are associated with the
 * snake.
 */
struct Tile {
    int row = 0;
    int col = 0;
    Direction direction = Direction::kNone;

    /* it's hacky but we exclude the direction in the equality comparison
     * because we want to be able to compare snake tiles with target tiles which
     * have no direction */
    friend bool operator==(const Tile& a, const Tile& b) {
        return ((a.row == b.row) && (a.col == b.col));
    }
};

using Snake = std::vector<Tile>;
using Targets = std::vector<Tile>;

class SnakeGame {
   public:
    /** Spawn a snake and target on a screen with the parameter dimensions. */
    explicit SnakeGame(const ScreenDimension& dim);

    SnakeGame() = delete;
    ~SnakeGame() = default;
    SnakeGame(const SnakeGame&) = default;
    SnakeGame& operator=(const SnakeGame&) = default;
    SnakeGame(SnakeGame&&) = default;
    SnakeGame& operator=(SnakeGame&&) = default;

    int GetScore() const { return score_; }
    ScreenDimension GetScreenDimension() const { return screen_dim_; }
    const Tile& GetTargetTile() const { return targets_[curr_target_]; }
    const Snake& GetSnake() const { return snake_; }
    bool GameOver() const { return game_over_; }

    /**
     * Advance the snake one game tick in the parameter direction.
     *
     * If the game has ended, Tick() will do nothing.
     *
     * @param[in] new_direction Direction in which the snake shall move in this
     * game tick.
     */
    void Tick(const Direction& new_direction);

    /** Reset game state and spawn a new snake and target. */
    void Reset();

   private:
    static const int kScoreIncrement = 10;

    /** Spawn a single snake head Tile with a random direction at the screen
     * center. */
    void SpawnSnake();

    /** Advance the snake one Tile in the parameter direction. */
    void MoveSnake(const Direction& new_direction);

    /** Append a new snake Tile to the end of the snake. */
    void ExtendSnake();

    /**
     * Return true if the game has ended in a loss.
     *
     * The game can end under the following conditions:
     *   (1) The snake head has intersected the snake body at some Tile.
     *   (2) The snake has attempted to go beyond the screen bounds.
     *
     * @returns true if the game has ended in a loss.
     */
    bool IsGameOver() const;

    /** Return true if the snake has won by populating every screen Tile. */
    bool SnakeWins() const;

    bool game_over_;
    int score_;
    ScreenDimension screen_dim_;
    Snake snake_;
    int curr_target_;
    Targets targets_;
};

}  // namespace game
}  // namespace snake

#endif
