#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "EvaluationWeights.h"

/**
 * @brief Represents the 2048 game board.
 *
 * Handles all core game logic including tile movement, merging,
 * scoring, spawning, and board evaluation for bot decision making.
 * Inherits from sf::Drawable and sf::Transformable for SFML rendering.
 */
class Board: public sf::Drawable, public sf::Transformable {
private:
    /** @brief Current score accumulated from merges. */
    int _score = 0;

    /** @brief 4x4 grid of tile values. 0 represents an empty cell. */
    std::array<std::array<int, 4>, 4> _tiles = {};

    /** @brief Display name shown above the board (typically the bot name). */
    std::string _gameName;

    /** @brief Renders the board, tiles, score and name to the SFML render target. */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    /** @brief Top-left position of the board in window coordinates. */
    sf::Vector2f _position;

    /** @brief Size of each tile in pixels. */
    float _size;

    /**
     * @brief The four possible move directions.
     */
    enum class Direction {
        Left,
        Right,
        Up,
        Down
    };

    /**
     * @brief Constructs a board at a given position with a display name.
     * @param position Top-left corner of the board in window space.
     * @param size Pixel size of each tile.
     * @param gameName Name displayed above the board.
     */
    Board(const sf::Vector2f& position, float size, const std::string& gameName);

    ~Board() = default;

    /**
     * @brief Returns the color for a tile based on its value.
     * @param value Tile value (e.g. 2, 4, 8...).
     * @return SFML color corresponding to the value.
     */
    sf::Color getTileColor(int value) const;

    /** @brief Prints the tile grid to stdout for debugging. */
    void printInFormat() const;

    /**
     * @brief Adds a value to the current score.
     * @param value Amount to add (typically the merged tile value).
     */
    void increaseScore(int value);

    /** @brief Returns the current score. */
    int giveScore() const;

    /**
     * @brief Spawns a new tile (90% chance of 2, 10% chance of 4)
     * at a random empty cell.
     */
    void spawnTile();

    /**
     * @brief Checks whether any move is possible.
     * @return True if at least one empty cell or mergeable pair exists.
     */
    bool canMove() const;

    /**
     * @brief Applies a move in the given direction.
     * Slides and merges tiles, increases score on merges.
     * Spawns a new tile after the move if not simulating.
     * @param direction The direction to slide tiles.
     * @param simulating If true, skips tile spawn (used during bot search).
     * @return True if the board state changed.
     */
    bool processInput(const Direction& direction, bool simulating);

    /** @brief Returns a copy of the current tile grid. */
    std::array<std::array<int, 4>, 4> getTiles() const;

    /**
     * @brief Heuristic evaluation of the board state for bot's decision making.
     * Combines empty cells, monotonicity, snake weighting,
     * merge potential, corner bonus and max tile into a single score.
     * @return Higher values indicate more favorable board states.
     */
    double evaluate(const EvaluationWeights& weights = EvaluationWeights{}) const;

    /**
     * @brief Sets the display name shown above the board.
     * @param gameName Name to display.
     */
    void setGameName(const std::string& gameName);

    /**
     * @brief Directly sets a tile value at the given position.
     * Used by Expectimax bot to simulate tile spawns during search.
     * @param row Row index (0-3).
     * @param col Column index (0-3).
     * @param value Value to place.
     */
    void setTile(int row, int col, int value);

    /**
     * @brief Counts the number of empty cells on the board.
     * Used by Expectimax to calculate spawn probabilities.
     * @return Number of tiles with value 0.
     */
    int getNumberOfEmptyTiles() const;
};