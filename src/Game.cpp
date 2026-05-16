#include "Game.h"

// Game::Game(): _board(Board({100, 200}, 100.f)) {
//     _board.spawnTile();
//     _board.spawnTile();
// }

Game::Game(const sf::Vector2f &position, std::string gameName): _board({0, 0}, 100.f, gameName) {
    _board.setPosition({position.x, position.y});

    _board.spawnTile();
    _board.spawnTile();
}

Game::Game(const sf::Vector2f &position, const Board &board, std::string gameName)
    : _board(board){  // ← copy, don't move yet
    _board.setGameName(gameName);
    _board.setPosition({position.x, position.y});

}

bool Game::processInput(const Board::Direction &direction, const bool simulating) {
    return _board.processInput(direction, simulating);

}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(_board);
}

Board Game::getBoard() const{
    return _board;
}

void Game::setGameName(const std::string &gameName) {
    _board.setGameName(gameName);
}

bool Game::canMove() const {
    return _board.canMove();
}

int Game::getScore() const {
    return _board.giveScore();
}







