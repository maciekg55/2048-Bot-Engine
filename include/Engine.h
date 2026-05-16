#pragma once
#include "Game.h"
#include <vector>
#include "GreedyBot.h"
#include "RandomBot.h"
#include "SearchBot.h"

/**
 * @brief Core application engine managing the game loop and rendering.
 *
 * Owns the SFML window, all active Game instances, and their
 * associated bots. Handles the event loop, bot update ticks,
 * and rendering. Supports both manual play and bot-driven modes.
 */
class Engine {
private:

    /** @brief The SFML render window. */
    sf::RenderWindow _window;

    /** @brief All active game instances displayed side by side. */
    std::vector<Game> _games;

    /**
     * @brief One bot per game, matched by index.
     * Each bot is owned exclusively by the engine.
     */
    std::vector<std::unique_ptr<Bot>> _bots;

    /** @brief Whether bot control is active. False enables manual keyboard input. */
    bool _botEnabled;

    /**
     * @brief Timer controlling bot move frequency.
     * Bots make a move every 120ms to allow visual observation.
     */
    sf::Clock _botClock;

    /** @brief Delay in milliseconds between bot moves in visual mode. */
    int _tickMs = 120;

public:
    /**
     * @brief Constructs a manual play engine with a single game and no bot.
     * Player controls the game via keyboard (WASD).
     */
    Engine();

    /**
     * @brief Constructs an engine with a single game and a SearchBot.
     * @param botEnabled If true, enables bot control.
     */
    Engine(bool botEnabled);

    /**
     * @brief Constructs an engine with one game per bot.
     * Window width scales dynamically based on the number of bots.
     * All games share the same initial board layout for fair comparison.
     * @param bots Vector of bots, one per game. Ownership is transferred.
     * @param tickMS integer indicating time passing between each frame in the visual mode
     */
    Engine(std::vector<std::unique_ptr<Bot>> bots, int tickMs = 120);

    /** @brief Starts the main game loop. Runs until the window is closed. */
    void run();

private:
    /** @brief Polls and handles SFML events including window close and keyboard input. */
    void handleEvent();

    /**
     * @brief Ticks all bots and applies their moves to their respective games.
     * Called every frame but only executes moves after the 120ms clock interval.
     */
    void update();

    /** @brief Clears the window, draws all games, and displays the frame. */
    void render();
};