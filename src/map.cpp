#include <map.hpp>

Map::Map(sf::Vector2u mapSizeInput, int squareSideInput, std::vector<Obstacle> obstaclesInput)
    : mapSize(mapSizeInput),
      squareSide(squareSideInput),
      obstacles(obstaclesInput) {
}

void Map::computePassMask(float robotRadius){
    passMask.clear();
    for (unsigned int y = 0; y < mapSize.y; y += squareSide){
        passMask.push_back({});
        for (unsigned int x = 0; x < mapSize.x; x += squareSide){
            passMask.back().push_back(1);
            for (auto obstacle : obstacles){
                bool xInRect = obstacle.getPosition().x - robotRadius - squareSide <= x && x < (obstacle.getPosition().x + obstacle.getWidth() + robotRadius);
                bool yInRect = obstacle.getPosition().y - robotRadius - squareSide <= y && y < (obstacle.getPosition().y + obstacle.getHeight() + robotRadius);
                if (xInRect && yInRect) {
                    passMask.back().back() = 0;
                    break;
                }
            }
        }
    }
}

void Map::computeObstacleMask(){
    obstacleMask.clear();
    for (unsigned int y = 0; y < mapSize.y; y += squareSide){
        obstacleMask.push_back({});
        for (unsigned int x = 0; x < mapSize.x; x += squareSide){
            obstacleMask.back().push_back(1);
            for (auto obstacle : obstacles){
                bool xInRect = obstacle.getPosition().x <= x && x < (obstacle.getPosition().x + obstacle.getWidth());
                bool yInRect = obstacle.getPosition().y <= y && y < (obstacle.getPosition().y + obstacle.getHeight());
                if (xInRect && yInRect) {
                    obstacleMask.back().back() = 0;
                    break;
                }
            }
        }
    }
}

void Map::drawObstacles(sf::RenderWindow& window){
    for (auto obstacle : obstacles){
        window.draw(obstacle.getBodyRect());
    }
}

void Map::drawAvoidanceField(sf::RenderWindow& window, sf::Image windowCopy){
    for (auto square : obstacleRects) window.draw(square);
}

void Map::makeAvoidanceRects(){
    for (unsigned int i = 0; i < passMask.size(); i++){
        auto row = passMask[i];
        for (unsigned j = 0; j < row.size(); j++){
            bool isSquareOk = row[j];

            if (!isSquareOk) {
                sf::Vector2f squareTopLeft = {j * squareSide, i * squareSide};
                sf::RectangleShape square({squareSide, squareSide});
                square.setPosition(squareTopLeft);
                square.setFillColor(sf::Color(255, 0, 0, 100));
                obstacleRects.push_back(square);
            }
        }
    }
}

void Map::addColorToSquare(sf::RenderWindow& window, sf::Image windowCopy, sf::Vector2f leftTop, sf::Color colorToAdd){
    int x = leftTop.x;
    int y = leftTop.y;

    int xMax = window.getSize().x;
    int yMax = window.getSize().y;

    if (x < 0 || x >= xMax || y < 0 || y >= yMax) return;
    sf::Color color = windowCopy.getPixel({x,y});

    color = color + colorToAdd;
    color = color - sf::Color(50, 50, 50, 0);
    color.a = 255;

    sf::RectangleShape square({squareSide, squareSide});
    square.setPosition(leftTop);
    square.setFillColor(color);

    window.draw(square);
}

void Map::addObstacle(Obstacle obstacle){
    obstacles.push_back(obstacle);
}

std::vector<Obstacle> Map::getObstacles(){
    return obstacles;
}

const mask2& Map::getPassMask() const {
    return passMask;
}

const mask2& Map::getObstacleMask() const {
    return obstacleMask;
}

int Map::getSquareSide(){
    return squareSide;
}

int Map::getSquareSide() const{
    return squareSide;
}

sf::Vector2u Map::getMapSize(){
    return mapSize;
}

bool Map::isOnMap(sf::Vector2f pos){
    return (0 <= pos.x && pos.x < mapSize.x) && (0 <= pos.y && pos.y < mapSize.y);
}
