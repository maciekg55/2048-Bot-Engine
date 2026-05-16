#include "Bot.h"

Bot::Bot(const std::string& name) : _botName(name) {}

Board::Direction Bot::getNextMove(const Game &game){}

std::string Bot::getBotName() const {
    return _botName;
}
