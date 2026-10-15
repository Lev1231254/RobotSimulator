#ifndef MAP_DATA_H
#define MAP_DATA_H
#include <objects.hpp>
#include <vector>

sf::Color obstacleColor = sf::Color(230, 230, 230, 255);

std::vector<Obstacle> obstacles1 = {
    Obstacle({640,960}, {400, 96}, obstacleColor),
    Obstacle({640,320}, {96, 640}, obstacleColor),
    // snake
    Obstacle({96, 96}, {96, 16}, obstacleColor),
    Obstacle({96, 96}, {16, 96}, obstacleColor),
    Obstacle({96 + 96 - 16, 16}, {16, 96}, obstacleColor),
    Obstacle({96 + 96 - 16, 16}, {96, 16}, obstacleColor),
    // unreachable square
    Obstacle({160, 160}, {96, 16}, obstacleColor),
    Obstacle({160, 160}, {16, 96}, obstacleColor),
    Obstacle({160, 160 + 96 - 16}, {96, 16}, obstacleColor),
    Obstacle({160 + 96 - 16, 160}, {16, 96}, obstacleColor)
};  

#endif