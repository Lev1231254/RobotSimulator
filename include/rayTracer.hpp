#ifndef RAY_TRACER_HPP
#define RAY_TRACER_HPP

#include <structs.hpp>

class Map;

ray posToRay(sf::Vector2f start, sf::Vector2f end);


class RayTracer {
    int squareSide;
    const mask2& occupancyMask;

    public:
        RayTracer(Map& map, const mask2& occupancyMaskInput);

        sf::Vector2f getFirstHit(ray &ray, Map& map);
        sf::Vector2f getFirstHitFinite(ray &ray, Map& map, sf::Vector2f end);
        
};

#endif