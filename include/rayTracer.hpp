#ifndef RAY_TRACER_HPP
#define RAY_TRACER_HPP

#include <structs.hpp>

class Map;

ray posToRay(sf::Vector2f start, sf::Vector2f end);


class RayTracer {
    int squareSide;
    sf::Vector2u mapSize;
    mask2 occupancyMask;

    private:
        sf::Vector2i getSigns(ray &ray);
        sf::Vector2f findOffset(sf::Vector2f pos, sf::Vector2i signs);
        

    public:
        RayTracer(Map &map, mask2 &occupancyMaskInput);

        sf::Vector2f getFirstHit(ray &ray, Map& map);
        
};

#endif