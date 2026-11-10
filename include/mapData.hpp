#ifndef MAP_DATA_H
#define MAP_DATA_H
#include <objects.hpp>
#include <vector>

sf::Color obstacleColor = sf::Color(230, 230, 230, 255);

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

Obstacle labyrinthUpperWestWall({224, 160}, {432, 16}, obstacleColor);
Obstacle labyrinthUpperCenterWall({848, 160}, {400, 16}, obstacleColor);
Obstacle labyrinthUpperEastWall({1456, 160}, {320, 16}, obstacleColor);
Obstacle labyrinthWestEntryWall({224, 176}, {16, 240}, obstacleColor);
Obstacle labyrinthNorthCenterDivider({688, 176}, {16, 256}, obstacleColor);
Obstacle labyrinthNorthEastDivider({1264, 176}, {16, 208}, obstacleColor);
Obstacle labyrinthMiddleWestWall({400, 400}, {448, 16}, obstacleColor);
Obstacle labyrinthMiddleCenterWall({960, 400}, {432, 16}, obstacleColor);
Obstacle labyrinthMiddleEastWall({1536, 400}, {224, 16}, obstacleColor);
Obstacle labyrinthWestCenterDivider({384, 416}, {16, 224}, obstacleColor);
Obstacle labyrinthEastCenterDivider({1424, 416}, {16, 224}, obstacleColor);
Obstacle labyrinthLowerWestWall({224, 656}, {384, 16}, obstacleColor);
Obstacle labyrinthLowerCenterWall({768, 656}, {480, 16}, obstacleColor);
Obstacle labyrinthLowerEastWall({1360, 656}, {416, 16}, obstacleColor);
Obstacle labyrinthSouthWestDivider({624, 528}, {16, 224}, obstacleColor);
Obstacle labyrinthSouthEastDivider({1280, 528}, {16, 272}, obstacleColor);
Obstacle labyrinthWestSteppingStoneA({704, 464}, {48, 48}, obstacleColor);
Obstacle labyrinthWestSteppingStoneB({752, 528}, {48, 48}, obstacleColor);
Obstacle labyrinthWestSteppingStoneC({800, 592}, {48, 48}, obstacleColor);
Obstacle labyrinthEastCrossIslandVertical({1088, 464}, {32, 128}, obstacleColor);
Obstacle labyrinthEastCrossIslandHorizontal({1040, 512}, {128, 32}, obstacleColor);

std::vector<Obstacle> labyrinthMap = {
    labyrinthUpperWestWall,
    labyrinthUpperCenterWall,
    labyrinthUpperEastWall,
    labyrinthWestEntryWall,
    labyrinthNorthCenterDivider,
    labyrinthNorthEastDivider,
    labyrinthMiddleWestWall,
    labyrinthMiddleCenterWall,
    labyrinthMiddleEastWall,
    labyrinthWestCenterDivider,
    labyrinthEastCenterDivider,
    labyrinthLowerWestWall,
    labyrinthLowerCenterWall,
    labyrinthLowerEastWall,
    labyrinthSouthWestDivider,
    labyrinthSouthEastDivider,
    labyrinthWestSteppingStoneA,
    labyrinthWestSteppingStoneB,
    labyrinthWestSteppingStoneC,
    labyrinthEastCrossIslandVertical,
    labyrinthEastCrossIslandHorizontal
};

std::vector<Obstacle> emptyMap = {};

#endif