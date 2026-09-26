#ifndef DRAW_H
#define DRAW_H

#include <SFML/Graphics.hpp>
#include "robot.hpp"

void draw_robot(sf::RenderWindow& window, Robot robot);
void makeSquareGreener(sf::RenderWindow & window, sf::Image windowCopy, sf::Vector2f leftTop, float sideLen);

#endif
