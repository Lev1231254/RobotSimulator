#include <rayTracer.hpp>
#include <objects.hpp>
#include <utils.hpp>
#include <cmath>
#include <limits>

RayTracer::RayTracer(
    Map& map,
    const mask2& occupancyMaskInput,
    bool outsideMaskIsFreeInput)
    : squareSide(map.getSquareSide()),
      occupancyMask(occupancyMaskInput),
      outsideMaskIsFree(outsideMaskIsFreeInput) {}


ray posToRay(sf::Vector2f start, sf::Vector2f end){
    ray ray;
    ray.start = start;
    ray.direction = end - start;
    ray.direction = normalize(ray.direction);
    ray.angle = sf::radians(atan(ray.direction.y / ray.direction.x));

    return ray;
}

sf::Vector2f RayTracer::getFirstHit(ray &ray, Map& map){
    ray.direction = normalize(ray.direction);

    if (squareSide <= 0 || !map.isOnMap(ray.start)
        || (ray.direction.x == 0.f && ray.direction.y == 0.f)) {
        return {-1.f, -1.f}; // not on the map
    }

    int col = static_cast<int>(ray.start.x) / squareSide;
    int row = static_cast<int>(ray.start.y) / squareSide;
    const int stepX = (ray.direction.x > 0.f) - (ray.direction.x < 0.f);
    const int stepY = (ray.direction.y > 0.f) - (ray.direction.y < 0.f);
    const float infinity = std::numeric_limits<float>::infinity();
    const float deltaX = stepX == 0 ? infinity : squareSide / std::abs(ray.direction.x);
    const float deltaY = stepY == 0 ? infinity : squareSide / std::abs(ray.direction.y);

    if (stepX < 0 && ray.start.x == col * squareSide) --col;
    if (stepY < 0 && ray.start.y == row * squareSide) --row;

    const float nextX = stepX > 0 ? (col + 1) * squareSide : col * squareSide;
    const float nextY = stepY > 0 ? (row + 1) * squareSide : row * squareSide;
    float maxX = stepX == 0 ? infinity : (nextX - ray.start.x) / ray.direction.x;
    float maxY = stepY == 0 ? infinity : (nextY - ray.start.y) / ray.direction.y;
    float distance = 0.f;

    while (true) {
        const sf::Vector2f currentPos = {
            ray.start.x + ray.direction.x * distance,
            ray.start.y + ray.direction.y * distance
        };
        if (!map.isOnMap(currentPos)) return {-1.f, -1.f};

        const int maskRow = row;
        const int maskCol = col;
        const bool inMask = maskRow >= 0
            && static_cast<std::size_t>(maskRow) < occupancyMask.size()
            && maskCol >= 0
            && static_cast<std::size_t>(maskCol) < occupancyMask[maskRow].size();
        if (!inMask && !outsideMaskIsFree) return {-1.f, -1.f};
        if (inMask && !occupancyMask[maskRow][maskCol]) {
            return currentPos;
        }

        if (maxX < maxY) {
            distance = maxX;
            col += stepX;
            maxX += deltaX;
        } else if (maxY < maxX) {
            distance = maxY;
            row += stepY;
            maxY += deltaY;
        } else {
            distance = maxX;
            col += stepX;
            row += stepY;
            maxX += deltaX;
            maxY += deltaY;
        }
    }
}


sf::Vector2f RayTracer::getFirstHitFinite(ray &ray, Map& map, sf::Vector2f end){
    ray.direction = normalize(ray.direction);
    if (squareSide <= 0 || !map.isOnMap(ray.start)
        || (ray.direction.x == 0.f && ray.direction.y == 0.f)) {
        return {-1.f, -1.f}; // not on the map
    }

    int col = static_cast<int>(ray.start.x) / squareSide;
    int row = static_cast<int>(ray.start.y) / squareSide;
    const int stepX = (ray.direction.x > 0.f) - (ray.direction.x < 0.f);
    const int stepY = (ray.direction.y > 0.f) - (ray.direction.y < 0.f);
    const float infinity = std::numeric_limits<float>::infinity();
    const float deltaX = stepX == 0 ? infinity : squareSide / std::abs(ray.direction.x);
    const float deltaY = stepY == 0 ? infinity : squareSide / std::abs(ray.direction.y);

    if (stepX < 0 && ray.start.x == col * squareSide) --col;
    if (stepY < 0 && ray.start.y == row * squareSide) --row;

    const float nextX = stepX > 0 ? (col + 1) * squareSide : col * squareSide;
    const float nextY = stepY > 0 ? (row + 1) * squareSide : row * squareSide;
    float maxX = stepX == 0 ? infinity : (nextX - ray.start.x) / ray.direction.x;
    float maxY = stepY == 0 ? infinity : (nextY - ray.start.y) / ray.direction.y;
    float distance = 0.f;
    float totalDistance = getDistance(ray.start, end);

    while (distance < totalDistance) {
        const sf::Vector2f currentPos = {
            ray.start.x + ray.direction.x * distance,
            ray.start.y + ray.direction.y * distance
        };
        if (!map.isOnMap(currentPos)) return {-1.f, -1.f};

        const int maskRow = row;
        const int maskCol = col;
        const bool inMask = maskRow >= 0
            && static_cast<std::size_t>(maskRow) < occupancyMask.size()
            && maskCol >= 0
            && static_cast<std::size_t>(maskCol) < occupancyMask[maskRow].size();
        if (!inMask && !outsideMaskIsFree) return {-1.f, -1.f};
        if (inMask && !occupancyMask[maskRow][maskCol]) {
            return currentPos;
        }


        if (maxX < maxY) {
            distance = maxX;
            col += stepX;
            maxX += deltaX;
        } else if (maxY < maxX) {
            distance = maxY;
            row += stepY;
            maxY += deltaY;
        } else {
            distance = maxX;
            col += stepX;
            row += stepY;
            maxX += deltaX;
            maxY += deltaY;
        }
    }
    return {-1, -1};
}