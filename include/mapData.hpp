#ifndef MAP_DATA_H
#define MAP_DATA_H
#include <objects.hpp>
#include <vector>


std::vector<Obstacle> obstacles1 = {
    Obstacle({640,960}, {400, 96}, sf::Color::White),
    Obstacle({640,320}, {96, 640}, sf::Color::White),
    Obstacle({96,96}, {96, 96}, sf::Color::White)};

#endif