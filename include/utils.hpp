#ifndef UTILS_H
#define UTILS_H

#include <robot.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>

void scanAndMove(Robot& robot, sf::RectangleShape obstacle, float movement_speed, float turning_speed);
float clamp(float value, float min, float max);
bool circleIntersectsRect(sf::CircleShape circle, sf::RectangleShape rect);

#endif