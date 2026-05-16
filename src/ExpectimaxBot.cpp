#include "ExpectimaxBot.h"

ExpectimaxBot::ExpectimaxBot() : Bot("Expectimax Bot") {}

Board::Direction ExpectimaxBot::getNextMove(const Game &game) {

    int depth = 4;

    const std::array<Board::Direction, 4> directions = {
        Board::Direction::Left,
        Board::Direction::Right,
        Board::Direction::Up,
        Board::Direction::Down
    };

    Board::Direction bestMove = Board::Direction::Left;
    double bestScore = -9999;

    for (auto& direction: directions) {

        Board copy = game.getBoard();
        bool moved = copy.processInput(direction, true);
        if (!moved) continue;

        double score = expectimax(copy, depth-1, NodeType::Chance, 1.0);
        if (score>bestScore) {
            bestScore = score;
            bestMove = direction;
        }
    }
    return bestMove;
}

double ExpectimaxBot::expectimax(const Board &board, int depth, const NodeType &nodeType, double probability) {
    if (probability < 0.0001) return board.evaluate();
    if (depth == 0 || !board.canMove()) return board.evaluate();

    const std::array<Board::Direction, 4> directions = {
        Board::Direction::Left,
        Board::Direction::Right,
        Board::Direction::Up,
        Board::Direction::Down
    };

    if (nodeType == NodeType::Max) {
        double bestScore = -9999;
        for (auto& direction : directions) {
            Board copy = board;
            bool moved = copy.processInput(direction, true);
            if (!moved) continue;

            double score = expectimax(copy, depth-1, NodeType::Chance, probability);
            if (score>bestScore) {
                bestScore = score;
            }
        }
        return bestScore;
    }
    else {
        double totalScore = 0;
        int numberOfEmptyCells = board.getNumberOfEmptyTiles();
        double probabilityPerCell = 1.0 / numberOfEmptyCells;
        auto tiles = board.getTiles();
        for (int row = 0; row<4; row++) {
            for (int col = 0; col<4; col++) {
                if (tiles[row][col]!= 0) continue;

                Board copy2 = board;
                copy2.setTile(row, col, 2);
                double score2 = expectimax(copy2, depth-1, NodeType::Max, probability * 0.9 * probabilityPerCell);

                Board copy4 = board;
                copy4.setTile(row, col, 4);
                double score4 = expectimax(copy4, depth-1, NodeType::Max, probability * 0.1 * probabilityPerCell);


                totalScore+=probabilityPerCell * (0.9 * score2 + 0.1*score4);
            }
        }
        return totalScore;
    }

}


