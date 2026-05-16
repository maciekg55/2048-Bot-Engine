#pragma once
#include "Bot.h"

class GreedyBot: public Bot {
public:
    GreedyBot();

    ~GreedyBot() override =  default;

    Board::Direction getNextMove(const Game &game) override;

};