#pragma once
#include "Bot.h"

class MCTSBot: public Bot {
public:

    enum class NodeType {
        Chance,
        Max,
    };

    MCTSBot();

    ~MCTSBot() override =  default;

    Board::Direction getNextMove(const Game &game) override;

private:

    double simulate(Board board, int maxDepth);
};