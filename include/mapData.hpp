#ifndef MAP_DATA_H
#define MAP_DATA_H
#include <objects.hpp>
#include <vector>

sf::Color obstacleColor = sf::Color(200, 200, 200, 255);

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


Obstacle labyrinthUpperWestWall({224, 160}, {480, 16}, obstacleColor);
Obstacle labyrinthUpperCenterWall({880, 160}, {400, 16}, obstacleColor);
Obstacle northEastCornerUpperWall({1456, 160}, {320, 16}, obstacleColor);
Obstacle labyrinthWestEntryWall({224, 176}, {16, 240}, obstacleColor);
Obstacle zigZagWest({688, 176}, {16, 256}, obstacleColor);
Obstacle zigZagEast({1264, 176}, {16, 256}, obstacleColor);
Obstacle tiplePointUpperWall({416, 416}, {448, 16}, obstacleColor);
Obstacle tiplePointLowerWall({80, 672}, {784, 16}, obstacleColor);
Obstacle kishkaVertical1({256, 672}, {16, 240}, obstacleColor);
Obstacle kishkaVertical2({608, 672}, {16, 240}, obstacleColor);
Obstacle kishkaVertical3({432, 864}, {16, 240}, obstacleColor);
Obstacle kishkaVertical4({784, 864}, {16, 240}, obstacleColor);
Obstacle greatSouthWall({80, 1088}, {1760, 16}, obstacleColor);
Obstacle trapWallA({1264, 416}, {368, 16}, obstacleColor);
Obstacle trapWallB({1264, 656}, {368, 16}, obstacleColor);
Obstacle trapWallC({1264, 896}, {368, 16}, obstacleColor);
Obstacle trapEastWallRight({1824, 368}, {16, 736}, obstacleColor);
Obstacle trapEastWallLeft({1632, 416}, {16, 496}, obstacleColor);
Obstacle labyrinthWestSteppingStoneA({704, 464}, {48, 48}, obstacleColor);
Obstacle labyrinthWestSteppingStoneB({752, 528}, {48, 48}, obstacleColor);
Obstacle labyrinthWestSteppingStoneC({800, 592}, {48, 48}, obstacleColor);
Obstacle crossVertical(  {1008 + 32, 464     }, {32, 96}, obstacleColor);
Obstacle crossHorizontal({1008,      464 + 32}, {96, 32}, obstacleColor);
Obstacle crossBall(      {1008 + 16, 464 + 16}, {64, 64}, obstacleColor);

std::vector<Obstacle> labyrinthMap = {
    labyrinthUpperWestWall,
    labyrinthUpperCenterWall,
    northEastCornerUpperWall,
    labyrinthWestEntryWall,
    zigZagWest,
    zigZagEast,
    tiplePointUpperWall,
    tiplePointLowerWall,
    kishkaVertical1,
    kishkaVertical2,
    kishkaVertical3,
    kishkaVertical4,
    trapWallA,
    trapWallB,
    trapWallC,
    greatSouthWall,
    trapEastWallRight,
    trapEastWallLeft,
    labyrinthWestSteppingStoneA,
    labyrinthWestSteppingStoneB,
    labyrinthWestSteppingStoneC,
    crossVertical,
    crossHorizontal,
    crossBall
};

std::vector<Obstacle> emptyMap = {};

#endif