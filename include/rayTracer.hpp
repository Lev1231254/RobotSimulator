#ifndef RAY_TRACER_HPP
#define RAY_TRACER_HPP

#include <utils.hpp>
#include <nodes.hpp>

struct ray{
    sf::Vector2f start;
    sf::Vector2f direction; //normialized vector
    sf::Angle angle;
};


ray posToRay(sf::Vector2f start, sf::Vector2f end);


class RayTracer {
    int squareSide;
    sf::Vector2u mapSize;
    mask2 occupancyMask;

    private:
        sf::Vector2i getSigns(ray &ray);
        sf::Vector2f findOffset(sf::Vector2f &pos, sf::Vector2i &signs);
        

    public:
        RayTracer(Map map, mask2 occupancyMaskInput)
            : squareSide(map.getSquareSide()),
            mapSize(map.getMapSize()),
            occupancyMask(occupancyMaskInput) {};

        sf::Vector2f getFirstHit(ray &ray, Map& map);
        
};

#endif