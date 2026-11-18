#ifndef CONTROLS_GUIDE_HPP
#define CONTROLS_GUIDE_HPP

#include <SFML/Graphics.hpp>

class ControlsGuide {
    sf::Font font;
    sf::RectangleShape background;
    sf::Text header;
    sf::Text headerHint;
    sf::Text leftText;
    sf::Text rightText;

public:
    ControlsGuide();
    void draw(sf::RenderWindow& window);
};

#endif
