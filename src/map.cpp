#include <map.hpp>

Map::Map(sf::Vector2u mapSizeInput, int squareSideInput, std::vector<Obstacle> obstaclesInput)
    : mapSize(mapSizeInput),
      squareSide(squareSideInput),
      obstacles(obstaclesInput) {
}

void Map::setupPassMask(float robotRadius){

    for (int row = 0; row < obstacleMask.size(); row ++){
        passMask.push_back({});
        for (int col = 0; col < obstacleMask[0].size(); col ++){
            passMask[row].push_back(0);
        }
    }
    updatePassMask(robotRadius);
}
void Map::passMaskOnes(){

    for (int row = 0; row < passMask.size(); row ++){
        for (int col = 0; col < passMask[0].size(); col ++){
            passMask[row][col] = 1;
        }
    }
}
void Map::updatePassMask(float robotRadius){
    passMaskOnes();
    float minDistance = robotRadius + squareSide;
    int minSquares = int(minDistance) / squareSide + 2;

    for (int row = 0; row < obstacleMask.size(); row ++){
        for (int col = 0; col < obstacleMask[0].size(); col ++){

            if (obstacleMask[row][col] == 0){
                for (int drow = -minSquares + 1; drow < minSquares; drow++){
                    for (int dcol = -minSquares + 1 ; dcol < minSquares; dcol++){
                        if (cordsInMask(obstacleMask, cords{row+drow, col+dcol}))
                            passMask[row + drow][col + dcol] = 0;
                    }
                }
            }
        }   
    }
}


void Map::setupObstacleMask(){
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

void Map::drawAvoidanceField(sf::RenderWindow& window){
    for (auto square : obstacleRects) window.draw(square);
}


void Map::setMiniMap(sf::Vector2f topLeft, sf::Vector2f size){
    miniMapBG.setSize(size);
    miniMapBG.setPosition(topLeft);
    miniMapBG.setFillColor(sf::Color::Blue);
    miniMapBG.setOutlineColor(sf::Color::White);
    miniMapBG.setOutlineThickness(3);
}

void Map::setObstMaskAt(cords cords, bool value){
    if (0 <= cords.row && cords.row < obstacleMask.size() && 0 <= cords.col && cords.col < obstacleMask[0].size())
        obstacleMask[cords.row][cords.col] = value;
    else printf("setObstMaskAt(): wrong cords given\n");
}

void Map::drawObstaclesMiniMap(sf::RenderWindow& window){

    int factor = mapSize.x / miniMapBG.getSize().x; // must always be integer number
    int squareSideMini = squareSide / factor;

    for (int row = 0; row < obstacleMask.size(); row++){
        for (int col = 0; col < obstacleMask[0].size(); col++){
            if (!obstacleMask[row][col]){
                sf::RectangleShape square({squareSideMini, squareSideMini});
                square.setPosition({miniMapBG.getPosition().x + col * squareSideMini, miniMapBG.getPosition().y + row * squareSideMini});
                square.setFillColor(sf::Color::White);
                window.draw(square);
            }
        }
    }
}

void Map::drawPassMaskMiniMap(sf::RenderWindow& window){
    int factor = mapSize.x / miniMapBG.getSize().x;
    int squareSideMini = squareSide / factor;

    for (int row = 0; row < passMask.size(); row++){
        for (int col = 0; col < passMask[0].size(); col++){
            if (passMask[row][col] == 0){
                sf::RectangleShape square({squareSideMini, squareSideMini});
                square.setPosition({miniMapBG.getPosition().x + col * squareSideMini, miniMapBG.getPosition().y + row * squareSideMini});
                square.setFillColor(sf::Color(255, 0, 0, 80));
                window.draw(square);
            }
        }
    }
}

void Map::drawMiniMap(sf::RenderWindow& window){
    window.draw(miniMapBG);
    drawObstaclesMiniMap(window);
    drawPassMaskMiniMap(window);
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

void Map::addColorToSquare(sf::RenderWindow& window, const sf::Image& windowCopy, sf::Vector2f leftTop, sf::Color colorToAdd){
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

const bool Map::getObstMaskAt(cords cords) const{
    if (0 <= cords.row && cords.row < obstacleMask.size() && 0 <= cords.col && cords.col < obstacleMask[0].size())
        return obstacleMask[cords.row][cords.col];
    printf("getObstMaskAt(): wrong cords given\n");
    return 0;
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

const sf::RectangleShape& Map::getMiniMap() const{
    return miniMapBG;
}

bool Map::isOnMap(sf::Vector2f pos){
    return (0 <= pos.x && pos.x < mapSize.x) && (0 <= pos.y && pos.y < mapSize.y);
}
