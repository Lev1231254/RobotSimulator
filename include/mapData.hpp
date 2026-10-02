#ifndef MAP_DATA_H
#define MAP_DATA_H
#include <objects.hpp>


Map map1 = {
    {
        Obstacle({0,0}, {20, 1000}, sf::Color::White),
        Obstacle({0,0}, {1000, 20}, sf::Color::White),
        Obstacle({100,100}, {200, 200}, sf::Color::White)
    }
};

#endif