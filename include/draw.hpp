#ifndef DRAW_H
#define DRAW_H

#include <SFML/Graphics.hpp>
#include <objects.hpp>

void drawRobot(sf::RenderWindow& window, Robot robot);
void makeSquareGreener(sf::RenderWindow & window, sf::Image windowCopy, sf::Vector2f leftTop, float sideLen);
void drawObstacles(sf::RenderWindow& window, std::vector<Obstacle> obstacles);

#endif
