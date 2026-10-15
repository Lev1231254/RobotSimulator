#include <rayTracer.hpp>




sf::Vector2f RayTracer::findOffset(sf::Vector2f pos, sf::Vector2i signs){
    sf::Vector2f entry = getSquareInGrid(squareSide, pos);

    sf::Vector2f offset = entry - pos;
    if (signs.x == 1) offset.x += squareSide;
    if (signs.y == 1) offset.y += squareSide;

    return offset;
    
}
sf::Vector2i RayTracer::getSigns(ray ray){
    sf::Vector2f d = ray.direction;
    sf::Vector2i res;
    res.x = (d.x >= 0) ? 1 : -1;
    res.y = (d.y >= 0) ? 1 : -1;

    return res;
}

cords RayTracer::getFirstHit(ray ray, Map& map){
    sf::Vector2f currPos = ray.start;
    cords cords = realPosToMatrix(getSquareInGrid(squareSide, currPos), squareSide);
    sf::Vector2f entryPos;

    while (map.isOnMap(currPos) && occupancyMask[cords.row][cords.col] == 0){

    }
    
}