#ifndef PF_MODE
#define PF_MODE

#include <SFML/Graphics.hpp>
#include <vector>
#include <objects.hpp>
#include <iostream>

std::vector<std::vector<bool>> getAvoidanceMatrix(std::vector<Obstacle> obstacles, int squareSide, sf::Vector2u gridSize); // matrix of the square-grid. 1 - can go; 0 - cant


#endif