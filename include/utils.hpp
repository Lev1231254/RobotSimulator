#ifndef UTILS_H
#define UTILS_H

#include <objects.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>

void scanAndMove(Robot& robot, Map map, float movement_speed, float turning_speed);
float clamp(float value, float min, float max);

bool circleIntersectsRect(sf::CircleShape circle, sf::RectangleShape rect);
bool robotCollidesObst(Robot robot, Obstacle obstacle);
bool robotCollidesObsts(Robot robot, std::vector<Obstacle> obstacles);

sf::Image getWindowImage(sf::RenderWindow& window);
sf::Vector2f getSquareInGrid(int squareSide, sf::Vector2f position);

float getDistance(sf::Vector2f pos1, sf::Vector2f pos2);

cords realPosToMatrix(sf::Vector2f pos, int squareSide);

float devideUntilLess(float num, float step);
sf::Vector2f normalize(sf::Vector2f vec);
void clampToSquare(sf::Vector2f &pos, sf::Vector2f &entryPos, int &squareSide);




#endif