#include "../include/MCTSBot.h"

MCTSBot::MCTSBot() : Bot("Monte Carlo Search Tree Bot") {}

Board::Direction MCTSBot::getNextMove(const Game &game) {

    const std::array<Board::Direction, 4> directions = {
        Board::Direction::Left,
        Board::Direction::Right,
        Board::Direction::Up,
        Board::Direction::Down
    };

    struct MoveStats {
        double totalScore = 0;
        int playouts = 0;
    };

    std::array<MoveStats, 4> stats;

    auto start = std::chrono::steady_clock::now();

    while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(100)) {
        int idx = 0;
        for (int i = 1; i<4; i++) {
            if (stats[i].playouts < stats[idx].playouts) idx = i;
        }
        Board copy = game.getBoard();
        bool moved = copy.processInput(directions[idx], true);
        if (!moved) {
            stats[idx].playouts++;
            continue;
        }

        double score = simulate(copy, 40);
        stats[idx].totalScore += score;
        stats[idx].playouts++;
    }

    Board::Direction bestMove = Board::Direction::Left;
    double bestAverage = -1;

    for (int i = 0; i< 4; i++) {
        if (stats[i].playouts == 0) continue;

        double average = stats[i].totalScore/ stats[i].playouts;
        if (average > bestAverage) {
            bestAverage = average;
            bestMove = directions[i];
        }
    }

    return bestMove;
}

double MCTSBot::simulate(Board board, int maxDepth) {
    thread_local  std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0,3);
    const std::array<Board::Direction, 4> directions = {
        Board::Direction::Left,
        Board::Direction::Right,
        Board::Direction::Up,
        Board::Direction::Down
    };

    int depth = 0;

    while (board.canMove() && depth < maxDepth) {
        Board::Direction direction = directions[dist(rng)];
        board.processInput(direction, false);
        depth++;
    }

    return board.evaluate();
}
