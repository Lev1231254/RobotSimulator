#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include <SFML/Graphics.hpp>

class Obstacle {
    sf::RectangleShape body;

public:
    Obstacle(sf::Vector2f pos, sf::Vector2f size, sf::Color color);
    sf::RectangleShape getBodyRect();
    sf::Vector2f getPosition() const;
    float getWidth() const;
    float getHeight() const;

    void setPosition(sf::Vector2f position);
    void setWidth(float width);
    void setHeight(float height);
    void setSize(sf::Vector2f size);
    void setColor(sf::Color colorInput);
};

#endif
