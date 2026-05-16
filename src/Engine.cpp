#include "Engine.h"
#include "Constants.h"

Engine::Engine() : _botEnabled(false) {
    _games.push_back(Game({100, 200}, "player"));
    _games[0].setGameName("Player");
}

Engine::Engine(const bool botEnabled) : _botEnabled(botEnabled) {

    _games.push_back(Game({100, 200}, "Search Bot"));

    _window.create(sf::VideoMode({static_cast<unsigned int>(600), Constants::General::WINDOW_HEIGHT}), "window", sf::Style::Close);
    if (botEnabled) {
        _bots.push_back(std::make_unique<SearchBot>());
        _games[0].setGameName(_bots[0]->getBotName());
        _botClock.start();
    }
}

Engine::Engine(std::vector<std::unique_ptr<Bot>> bots, int tickMs) : _botEnabled(true), _tickMs(tickMs){
    _games.push_back(Game({100, 200}, bots[0]->getBotName()));

    Board blueprint = _games[0].getBoard();
    for (int i = 1; i<bots.size(); i++) {
        blueprint.setGameName(bots[i]->getBotName());
        _games.push_back(

            Game({100 + 600.f*i, 200.f}, blueprint, bots[i]->getBotName()));

    }
    int windowWidth = (bots.size() * 600);
    _window.create(sf::VideoMode({static_cast<unsigned int>(windowWidth), Constants::General::WINDOW_HEIGHT}), "window", sf::Style::Close);
    _bots = std::move(bots);
    _botClock.start();
}



void Engine::run() {
    while (_window.isOpen()) {
        handleEvent();
        update();
        render();
    }
}

void Engine::handleEvent() {
    while (const auto event = _window.pollEvent()) {
        if (event -> is<sf::Event::Closed>() ) _window.close();
            if (!_botEnabled) {     // saying that only one can be played at a time therefore working on games[0]
                if (event->is<sf::Event::KeyPressed>()) {

                    auto key = event->getIf<sf::Event::KeyPressed>()->code;

                    if (key == sf::Keyboard::Key::A)
                        _games[0].processInput(Board::Direction::Left, false);

                    if (key == sf::Keyboard::Key::W)
                        _games[0].processInput(Board::Direction::Up, false);

                    if (key == sf::Keyboard::Key::S)
                        _games[0].processInput(Board::Direction::Down, false);

                    if (key == sf::Keyboard::Key::D)
                        _games[0].processInput(Board::Direction::Right, false);

                }
            }
    }
}


void Engine::render() {
    _window.clear(Constants::Colors::GAME_BACKGROUND_COLOR);
    for (auto& i : _games) {
        _window.draw(i);
    }
    _window.display();
}

void Engine::update() {
    if (_botEnabled) {
        if (_botClock.getElapsedTime().asMilliseconds() > _tickMs) {
            for (int i = 0; i< _games.size(); i++) {
                const auto move = _bots[i]->getNextMove(_games[i]);
                _games[i].processInput(move, false);
                _botClock.restart();

            }
        }
    }

}




