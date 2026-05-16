#pragma once
#include "Game.h"
#include <random>


class Bot {
protected:
    std::string _botName;

public:
    Bot(const std::string& name);

    virtual ~Bot() = default;

    virtual Board::Direction getNextMove(const Game& game) = 0;

    std::string getBotName() const;
};