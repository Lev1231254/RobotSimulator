#ifndef UTILS_H
#define UTILS_H


#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include <structs.hpp>

int sign(float num);
float clamp(float value, float min, float max);

bool circleIntersectsRect(sf::CircleShape circle, sf::RectangleShape rect);


sf::Image getWindowImage(sf::RenderWindow& window);
sf::Vector2f getSquareInGrid(int squareSide, sf::Vector2f position);

float getDistance(sf::Vector2f pos1, sf::Vector2f pos2);

cords posToCords(sf::Vector2f pos, int squareSide);
sf::Vector2f cordsToPos(cords cords, int squareSide);

float devideUntilLess(float num, float step);
sf::Vector2f normalize(sf::Vector2f vec);
void clampToSquare(sf::Vector2f &pos, sf::Vector2f &entryPos, int &squareSide);

float getAngleToPointf(sf::Vector2f start, sf::Vector2f end);
sf::Angle getAngleToPos(sf::Vector2f start, sf::Vector2f end);
bool anglesEqual(sf::Angle angle1, sf::Angle angle2);
void normalizeAngleDeg(float &angle);
void normalizeAngle(sf::Angle &angle);


#endif