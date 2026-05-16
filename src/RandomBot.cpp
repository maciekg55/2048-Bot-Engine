#include "RandomBot.h"

RandomBot::RandomBot() : Bot("Random Bot") {}

Board::Direction RandomBot::getNextMove(const Game &game) {
    thread_local std::mt19937 engine(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 3);

    switch (dist(engine)) {
        case 0: return Board::Direction::Left;
        case 1: return Board::Direction::Right;
        case 2: return Board::Direction::Up;
        case 3: return Board::Direction::Down;
    }

    return Board::Direction::Left;

}
