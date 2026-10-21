#ifndef UTILS_H
#define UTILS_H


#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include <structs.hpp>

float clamp(float value, float min, float max);

bool circleIntersectsRect(sf::CircleShape circle, sf::RectangleShape rect);


sf::Image getWindowImage(sf::RenderWindow& window);
sf::Vector2f getSquareInGrid(int squareSide, sf::Vector2f position);

float getDistance(sf::Vector2f pos1, sf::Vector2f pos2);

cords realPosToMatrix(sf::Vector2f pos, int squareSide);
sf::Vector2f matrixPosToReal(cords cords, int squareSide);

float devideUntilLess(float num, float step);
sf::Vector2f normalize(sf::Vector2f vec);
void clampToSquare(sf::Vector2f &pos, sf::Vector2f &entryPos, int &squareSide);

float getAngleToPointf(sf::Vector2f start, sf::Vector2f end);
sf::Angle getAngleToPoint(sf::Vector2f start, sf::Vector2f end);

#endif