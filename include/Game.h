#pragma once
#include "Board.h"

/**
 * @brief Represents a single 2048 game instance.
 *
 * Thin wrapper around Board that manages a game's lifecycle.
 * Handles initialization with tile spawning and delegates
 * all game logic to the underlying Board. Inherits sf::Drawable
 * for SFML rendering.
 */
class Game: public sf::Drawable{
private:
    /** @brief The underlying board managing all game state and logic. */
    Board _board;

    /** @brief Position of the game in window space. */
    sf::Vector2f _position;

public:

    /**
     * @brief Constructs a new game at the given position with a display name.
     * Spawns two initial tiles on the board.
     * @param position Top-left corner of the board in window space.
     * @param gameName Name displayed above the board.
     */
    Game(const sf::Vector2f& position, std::string gameName);

    /**
     * @brief Constructs a game from an existing board state.
     * Used by Engine to create games sharing the same initial layout
     * for fair benchmarking comparisons.
     * @param position Top-left corner of the board in window space.
     * @param board Existing board to copy state from.
     * @param gameName Name displayed above the board.
     */
    Game(const sf::Vector2f& position, const Board& board, std::string gameName);

    ~Game() = default;

    /**
     * @brief Delegates a move input to the underlying board.
     * @param direction Direction to slide tiles.
     * @param simulating If true, skips tile spawn (used during bot search).
     * @return True if the board state changed.
     */
    bool processInput(const Board::Direction& direction, bool simulating);

    /** @brief Renders the game by drawing its board to the render target. */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /** @brief Returns a copy of the underlying board. Used by bots to simulate moves. */
    Board getBoard() const;

    /**
     * @brief Sets the display name on the underlying board.
     * @param gameName Name to display above the board.
     */
    void setGameName(const std::string& gameName);

    /** @brief Returns true if any move is still possible on the board. */
    bool canMove() const;

    /** @brief Returns the current score from the underlying board. */
    int getScore() const;
};