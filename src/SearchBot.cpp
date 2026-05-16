#include "SearchBot.h"

SearchBot::SearchBot() : Bot("Search Bot") {}

Board::Direction SearchBot::getNextMove(const Game &game) {

    const int depth = 6;

    Board::Direction bestMove = Board::Direction::Left;
    int bestScore = -999999;

    const std::array<Board::Direction, 4> directions = {
        Board::Direction::Left,
        Board::Direction::Right,
        Board::Direction::Up,
        Board::Direction::Down
    };

    for (const auto& direction : directions) {

        Board copy = game.getBoard();

        bool moved = copy.processInput(direction, true);

        if (!moved) {
            continue;
        }

        int score = search(copy, depth - 1);

        if (score > bestScore) {
            bestScore = score;
            bestMove = direction;
        }
    }

    return bestMove;
}

int SearchBot::search(Board &board, const int depth) {
    if (depth == 0 || !board.canMove()) {
        return board.evaluate();
    }
    int bestScore = -9999;
    const std::array<Board::Direction, 4> directions = {
        Board::Direction::Left,
        Board::Direction::Right,
        Board::Direction::Up,
        Board::Direction::Down
    };

    for (const auto& direction : directions) {
        Board copy = board;
        bool moved = copy.processInput(direction, true);
        if (!moved) continue;

        int score = search(copy, depth-1);
        bestScore = std::max(bestScore, score);

    }
    return bestScore;
}
