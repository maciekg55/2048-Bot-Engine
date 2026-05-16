#include "GreedyBot.h"

GreedyBot::GreedyBot() : Bot("Greedy Bot") {}

Board::Direction GreedyBot::getNextMove(const Game &game) {
    int bestScore = -9999;
    Board::Direction bestMove = Board::Direction::Left;

    const std::array<Board::Direction, 4> directions = {
        Board::Direction::Left,
        Board::Direction::Right,
        Board::Direction::Up,
        Board::Direction::Down
    };



    for (auto& direction : directions) {
        Board copy = game.getBoard();
        bool moved = copy.processInput(direction, true);

        if (!moved) continue;

        double score = copy.evaluate();
        if (score > bestScore) {
            bestScore = score;
            bestMove = direction;
        }
    }

    return bestMove;

}
