#ifndef UTILS_H
#define UTILS_H


#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include <structs.hpp>

int sign(float num);
float clamp(float value, float min, float max);
float exponentialTransition(float value, float cap);

bool circleIntersectsRect(sf::CircleShape circle, sf::RectangleShape rect);
bool anglesEqual(sf::Angle angle1, sf::Angle angle2);
bool cordsInMask(const mask2& mask, cords cords);


sf::Image getWindowImage(sf::RenderWindow& window);
sf::Vector2f getSquareInGrid(int squareSide, sf::Vector2f position);
float getDistance(sf::Vector2f pos1, sf::Vector2f pos2);
float getDistance(cords pos1, cords pos2);

cords posToCords(sf::Vector2f pos, int squareSide);
sf::Vector2f cordsToPos(cords cords, int squareSide);
float angleToPointf(sf::Vector2f start, sf::Vector2f end);
sf::Angle angleToPos(sf::Vector2f start, sf::Vector2f end);
sf::Vector2u vec_f_to_u(sf::Vector2f vec);
sf::Vector2f vec_u_to_f(sf::Vector2u vec);
sf::Vector2f posToMiniMap(sf::Vector2f pos, sf::Vector2u size, const sf::RectangleShape& miniMap);

float devideUntilLess(float num, float step);
sf::Vector2f normalize(sf::Vector2f vec);
void clampToSquare(sf::Vector2f& pos, sf::Vector2f& entryPos, int& squareSide);
void normalizeAngleDeg(float& angle);
void normalizeAngle(sf::Angle& angle);


#endif