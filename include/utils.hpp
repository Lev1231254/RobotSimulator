#ifndef UTILS_H
#define UTILS_H

#include <objects.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>

void scanAndMove(Robot& robot, sf::RectangleShape obstacle, float movement_speed, float turning_speed);
float clamp(float value, float min, float max);
bool circleIntersectsRect(sf::CircleShape circle, sf::RectangleShape rect);
sf::Image getWindowImage(sf::RenderWindow& window);
sf::Vector2f getSquareInGrid(int squareSide, sf::Vector2i position);

#endif