#pragma once
#include "Bot.h"

class SearchBot: public Bot {
public:
    SearchBot();

    ~SearchBot() override =  default;

    Board::Direction getNextMove(const Game &game) override;

    int search(Board& board, int depth);

};