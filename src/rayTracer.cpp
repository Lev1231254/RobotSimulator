#include <rayTracer.hpp>
#include <objects.hpp>
#include <utils.hpp>

RayTracer::RayTracer(Map& map, mask2& occupancyMaskInput)
    : squareSide(map.getSquareSide()),
      mapSize(map.getMapSize()),
      occupancyMask(occupancyMaskInput) {}


ray posToRay(sf::Vector2f start, sf::Vector2f end){
    ray ray;
    ray.start = start;
    ray.direction = end - start;
    ray.direction = normalize(ray.direction);
    ray.angle = sf::radians(atan(ray.direction.y / ray.direction.x));

    return ray;
}

sf::Vector2f RayTracer::findOffset(sf::Vector2f pos, sf::Vector2i signs){
    sf::Vector2f entry = getSquareInGrid(squareSide, pos);

    sf::Vector2f offset = entry - pos;
    if (signs.x == 1) offset.x += squareSide;
    if (signs.y == 1) offset.y += squareSide;

    return offset;
    
}
sf::Vector2i RayTracer::getSigns(ray &ray){
    sf::Vector2f d = ray.direction;
    sf::Vector2i res;
    res.x = (d.x >= 0) ? 1 : -1;
    res.y = (d.y >= 0) ? 1 : -1;

    return res;
}

sf::Vector2f RayTracer::getFirstHit(ray &ray, Map& map){

    sf::Vector2f currPos = ray.start;
    sf::Vector2f entryPos = getSquareInGrid(squareSide, currPos);
    cords cords = realPosToMatrix(entryPos, squareSide);

    sf::Vector2i signs = getSigns(ray);
    sf::Vector2f offset;

    sf::Vector2f tmax;

    
    while (map.isOnMap(currPos) && occupancyMask[cords.row][cords.col] == 0){

        if (occupancyMask[cords.row][cords.col]) return currPos;

        //works only if direction is normalized
        tmax.x = offset.x / ray.direction.x;
        tmax.y = offset.y / ray.direction.y;
        
        if (tmax.x < tmax.y){
            currPos += ray.direction * tmax.x;
        }
        else {
            currPos += ray.direction * tmax.y;
        }


        currPos.x = round(currPos.x);
        currPos.y = round(currPos.y);

        entryPos = getSquareInGrid(squareSide, currPos);
        offset = findOffset(currPos, signs);
        cords = realPosToMatrix(entryPos, squareSide);
        
    }
    
    printf("Beam didn't hit anything inside the map\n");
    return {-1, -1};
}