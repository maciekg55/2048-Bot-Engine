#pragma once
#include "Bot.h"

class RandomBot: public Bot {
public:

    RandomBot();
    ~RandomBot() override =  default;

    Board::Direction getNextMove(const Game &game) override;





};