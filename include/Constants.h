#pragma once
#include <SFML/Graphics.hpp>

namespace Constants {
    struct General {
        static constexpr unsigned int WINDOW_HEIGHT = 1080;
    };
    struct Fonts {
        inline static const auto font = sf::Font("assets/ClearSans-Medium.ttf");
    };
    struct Colors {
        static constexpr auto GAME_BACKGROUND_COLOR = sf::Color(250, 248, 239);
        static constexpr auto TILE_BACKGROUND_COLOR = sf::Color(185, 169, 158);
        static constexpr auto TILE_BORDER_COLOR = sf::Color(156, 138, 124);
        static constexpr auto TEXT_COLOR_1 = sf::Color(117, 100, 81);
        static constexpr auto OCCUPIED_TILE_BACKGROUND_COLOR_1 = sf::Color(238, 228, 218);
    };

}