#pragma once
#include "Bot.h"

class ExpectimaxBot: public Bot {
public:

    enum class NodeType {
        Chance,
        Max,
    };

    ExpectimaxBot();

    ~ExpectimaxBot() override =  default;

    Board::Direction getNextMove(const Game &game) override;

private:

    double expectimax(const Board& board, int depth, const NodeType& nodeType, double probability);

};