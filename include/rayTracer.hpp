#ifndef RAY_TRACER_HPP
#define RAY_TRACER_HPP

#include <utils.hpp>

struct cords{
    int row;
    int col;
};

struct ray{
    sf::Vector2f start;
    sf::Vector2f direction; //normialized vector
};


ray posToRay(sf::Vector2f start, sf::Vector2f end);


class RayTracer {
    int squareSide;
    sf::Vector2u mapSize;
    mask2 occupancyMask;

    private:
        sf::Vector2i getSigns(ray ray);
        sf::Vector2f findOffset(sf::Vector2f pos, sf::Vector2i signs);
        sf::Vector2f findDeltas(ray ray);
        sf::Vector2f getNearest(ray ray, sf::Vector2f currPos);
        

    public:
        RayTracer(Map map, mask2 occupancyMaskInput)
            : squareSide(map.getSquareSide()),
            mapSize(map.getMapSize()),
            occupancyMask(occupancyMaskInput) {};

        cords getFirstHit(ray ray, Map& map)
        
};

#endif