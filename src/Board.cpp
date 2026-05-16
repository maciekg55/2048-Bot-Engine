#include "Board.h"
#include "Constants.h"
#include <iostream>

Board::Board(const sf::Vector2f& position, float size, const std::string& gameName) :  _position(position), _size(size), _gameName(gameName) {}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (int row = 0; row < 4; ++row){
        for (int col = 0; col < 4; ++col){

            int tile = _tiles[row][col];
            float outline = _size * 0.12f;

            sf::RectangleShape tileShape({_size - outline, _size - outline});
            sf::Vector2f tilePosition = {
                col * _size + _position.x + outline / 2.f,
                row * _size + _position.y + outline / 2.f
            };
            tileShape.setPosition(tilePosition);


            tileShape.setFillColor(getTileColor(tile));
            tileShape.setOutlineColor(Constants::Colors::TILE_BORDER_COLOR);
            tileShape.setOutlineThickness(outline);

            target.draw(tileShape, states);

            if (tile != 0) {
                std::string tileValue = std::to_string(tile);
                sf::Text text(Constants::Fonts::font, tileValue, _size/2.f);


                sf::FloatRect bounds = text.getLocalBounds();

                text.setOrigin(
                    {bounds.position.x + bounds.size.x / 2.f,
                    bounds.position.y + bounds.size.y / 2.f}
                );

                sf::Vector2f textPosition = {
                    tilePosition.x + (_size - outline) / 2.f,
                    tilePosition.y + (_size - outline) / 2.f
                };

                text.setPosition(textPosition);
                text.setFillColor(Constants::Colors::TEXT_COLOR_1);

                target.draw(text, states);

            }
        }
    }

    std::string scoreValue = std::to_string(_score);
    sf::Text scoreText(Constants::Fonts::font, scoreValue, _size/4.f);
    scoreText.setFillColor(Constants::Colors::TEXT_COLOR_1);

    sf::FloatRect scoreTextBounds = scoreText.getLocalBounds();

    scoreText.setPosition(
    {_position.x + (_size*4.f - scoreTextBounds.size.x) / 2.f - scoreTextBounds.position.x,
    _position.y + _size*4.f + 20.f}
    );

    target.draw(scoreText, states);



    sf::Text nameText(Constants::Fonts::font, _gameName, _size/4.f);
    nameText.setFillColor(Constants::Colors::TEXT_COLOR_1);

    sf::FloatRect nameTextBounds = nameText.getLocalBounds();

    nameText.setPosition(
    {_position.x + (_size*4.f - nameTextBounds.size.x) / 2.f - nameTextBounds.position.x,
    _position.y - _size/2.f}
    );

    target.draw(nameText, states);

}

void Board::printInFormat() const {
    for (auto& i : _tiles) {
        for (auto& j : i) {
            std::cout<<j<<" ";
        }
        std::cout<<"\n";
    }
}

bool Board::canMove() const {

    // empty tile exists
    for (const auto& row : _tiles) {
        for (int tile : row) {
            if (tile == 0) {
                return true;
            }
        }
    }

    // horizontal merges possible
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 3; col++) {

            if (_tiles[row][col] == _tiles[row][col + 1]) {
                return true;
            }

        }
    }

    // vertical merges possible
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 3; row++) {

            if (_tiles[row][col] == _tiles[row + 1][col]) {
                return true;
            }

        }
    }

    return false;
}

void Board::spawnTile() {
    std::vector<std::pair<int, int>> available;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (_tiles[row][col] == 0)
                available.emplace_back(row, col);
        }
    }

    if (available.empty()) {
        return;
    }

    thread_local std::mt19937 engine(std::random_device{}());

    std::uniform_int_distribution<int> indexDist(0, available.size() - 1);
    std::uniform_int_distribution<int> valueDist(0, 9);

    const auto [r, c] = available[indexDist(engine)];

    _tiles[r][c] = (valueDist(engine) < 9) ? 2 : 4;
}

bool Board::processInput(const Direction& direction, const bool simulating) {

    if (!canMove()) {

        return false;
    }

    bool moved = false;
    switch (direction) {
        case Direction::Left:
            for (auto& row : _tiles) {

                int lastNumberIndex = 0;
                bool mergedLast = false;

                for (int i = 0; i < 4; i++) {

                    if (row[i] == 0) continue;

                    if (lastNumberIndex > 0 && row[lastNumberIndex - 1] == row[i] && !mergedLast) {
                        row[lastNumberIndex - 1] *= 2;
                        mergedLast = true;
                        increaseScore(row[lastNumberIndex - 1]);
                        moved = true;
                    } else {
                        if (lastNumberIndex != i) {
                            moved = true;
                        }
                        row[lastNumberIndex] = row[i];
                        lastNumberIndex++;
                        mergedLast = false;

                    }

                    if (lastNumberIndex - 1 != i) {
                        row[i] = 0;
                        moved = true;
                    }
                }

                for (int i = lastNumberIndex; i < 4; i++) {
                    row[i] = 0;
                }
            }
        break;

        case Direction::Right:
            for (auto& row : _tiles) {
                int lastNumberIndex = 3;
                bool mergedLast = false;
                for (int i = 3; i >= 0; i--) {
                    if (row[i] == 0) continue;

                    if (lastNumberIndex < 3 && row[lastNumberIndex + 1] == row[i] && !mergedLast) {
                        row[lastNumberIndex + 1] *= 2;
                        mergedLast = true;
                        increaseScore(row[lastNumberIndex + 1]);
                        moved = true;
                    } else {
                        if (lastNumberIndex != i) {
                            moved = true;
                        }

                        row[lastNumberIndex] = row[i];
                        lastNumberIndex--;
                        mergedLast = false;
                    }

                    if (lastNumberIndex + 1 != i) {
                        row[i] = 0;
                        moved = true;
                    }

                }
                for (int i = lastNumberIndex; i >= 0; i--) {
                    row[i] = 0;
                }
            }
        break;

        case Direction::Down:
            for (int col = 0; col < 4; col++) {
                int lastNumberIndex = 3;
                bool mergedLast = false;
                for (int row = 3; row >= 0; row--) {
                    if (_tiles[row][col] == 0) continue;
                    if (lastNumberIndex < 3 && _tiles[lastNumberIndex + 1][col] == _tiles[row][col] && !mergedLast) {
                        _tiles[lastNumberIndex + 1][col] *= 2;
                        mergedLast = true;
                        increaseScore(_tiles[lastNumberIndex + 1][col]);
                        moved = true;
                    }
                    else {
                        if (lastNumberIndex != row) {
                            moved = true;
                        }
                        _tiles[lastNumberIndex][col] = _tiles[row][col];
                        lastNumberIndex--;
                        mergedLast = false;

                    }
                    if (lastNumberIndex + 1 != row) {
                        _tiles[row][col] = 0;
                        moved = true;
                    }
                }

                for (int row = lastNumberIndex; row >= 0; row--) {
                    _tiles[row][col] = 0;
                }
            }
        break;

        case Direction::Up:
            for (int col = 0; col < 4; col++) {
                int lastNumberIndex = 0;
                bool mergedLast = false;
                for (int row = 0; row < 4; row++) {
                    if (_tiles[row][col] == 0) continue;

                    if (lastNumberIndex > 0 && _tiles[lastNumberIndex - 1][col] == _tiles[row][col] && !mergedLast) {
                        _tiles[lastNumberIndex - 1][col] *= 2;
                        increaseScore(_tiles[lastNumberIndex - 1][col]);
                        mergedLast = true;
                        moved = true;
                    }
                    else {
                        if (lastNumberIndex != row) {
                            moved = true;
                        }
                        _tiles[lastNumberIndex][col] = _tiles[row][col];
                        lastNumberIndex++;
                        mergedLast = false;

                    }
                    if (lastNumberIndex - 1 != row) {
                        _tiles[row][col] = 0;
                        moved = true;
                    }
                }

                for (int row = lastNumberIndex; row < 4; row++) {
                    _tiles[row][col] = 0;
                }
            }
        break;
    }

    if (!simulating && moved) spawnTile();
    return moved;
}

sf::Color Board::getTileColor(int value) const {

    if (value == 0)
        return Constants::Colors::TILE_BACKGROUND_COLOR;
    if (value == 2)
        return Constants::Colors::OCCUPIED_TILE_BACKGROUND_COLOR_1;

    int level = std::log2(value);
    float t = std::min(1.f, level / 11.f); // normalize (2^11 = 2048 range)

    int r = 255;
    int g = static_cast<int>(255 * (1.f - t));
    int b = 0;

    return sf::Color(r, g, b);
}

std::array<std::array<int, 4>, 4> Board::getTiles() const {
    return _tiles;
}

void Board::increaseScore(const int value) {
    _score+=value;
}

int Board::giveScore() const {
    return _score;
}

double Board::evaluate(const EvaluationWeights &weights) const {
    double score = 0;

    int emptyCells = 0;
    int monotonicity = 0;
    int maxTile = 0;
    int snakeScore = 0;
    int mergePotential = 0;

    const int snakeWeights[4][4] = {
        {16, 15, 14, 13},
        {9, 10, 11, 12},
        {8, 7, 6, 5},
        {1, 2, 3, 4}
    };

    // Empty cells + max tile
    for (const auto& row : _tiles) {
        for (int tile : row) {
            if (tile == 0) {
                emptyCells++;
            }

            if (tile > maxTile) {
                maxTile = tile;
            }
        }
    }

    // Snake weighting
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            int tile = _tiles[row][col];
            if (tile == 0) continue;
            snakeScore += log2(tile) * snakeWeights[row][col];
        }
    }

    // Merge potential
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 3; col++) {
            if (_tiles[row][col] != 0 && _tiles[row][col] == _tiles[row][col+1])
                mergePotential += log2(_tiles[row][col]);
        }
    }
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 3; row++) {
            if (_tiles[row][col] != 0 && _tiles[row][col] == _tiles[row+1][col])
                mergePotential += log2(_tiles[row][col]);
        }
    }


    // Horizontal monotonicity
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 3; col++) {

            int left = _tiles[row][col];
            int right = _tiles[row][col + 1];

            if (left == 0 || right == 0) continue;

            int leftLog = log2(left);
            int rightLog = log2(right);

            monotonicity -= abs(leftLog - rightLog);
        }
    }

    // Vertical monotonicity
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 3; row++) {

            int top = _tiles[row][col];
            int bottom = _tiles[row + 1][col];

            if (top == 0 || bottom == 0) continue;

            int topLog = log2(top);
            int bottomLog = log2(bottom);

            monotonicity -= abs(topLog - bottomLog);
        }
    }

    // Corner bonus
    int highestCorner = std::max(
        std::max(_tiles[0][0], _tiles[0][3]),
        std::max(_tiles[3][0], _tiles[3][3])
    );

    int cornerBonus = 0;

    if (highestCorner == maxTile) {
        cornerBonus = maxTile;
    }


    score += emptyCells * weights.emptyCells;
    score += monotonicity * weights.monotonicity;
    score += snakeScore * weights.snake;
    score += cornerBonus * weights.cornerBonus;
    score += mergePotential * weights.mergePotential;
    score += log2(maxTile) * weights.maxTile;

    return score;
}

void Board::setGameName(const std::string &gameName) {
    _gameName = gameName;
}

void Board::setTile(int row, int col, int value) {
    _tiles[row][col] = value;
}

int Board::getNumberOfEmptyTiles() const{
    int number = 0;
    for (int row = 0; row<4; row++) {
        for (int col = 0; col<4; col++) {
            if (_tiles[row][col]== 0) number++;
        }
    }
    return number;
}



