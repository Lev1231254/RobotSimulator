#include <map.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>

namespace {
const char* cursorFontPath = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";

sf::Color obstacleAgeColor(int storedDiscoveryTime, float currentTimeSeconds) {
    const long long discoveredAtSeconds =
        -static_cast<long long>(storedDiscoveryTime) - 1;
    const float ageSeconds = std::max(
        0.f,
        currentTimeSeconds - static_cast<float>(discoveredAtSeconds));
    const float ageRatio = exponentialTransition(ageSeconds, 10.f);
    const auto red = static_cast<std::uint8_t>(std::lround(255.f * (1.f - ageRatio)));
    const auto blue = static_cast<std::uint8_t>(std::lround(255.f * ageRatio));
    return {red, 0, blue};
}
}

Map::Map(sf::Vector2u mapSizeInput, int squareSideInput, std::vector<Obstacle> obstaclesInput)
    : mapSize(mapSizeInput),
      squareSide(squareSideInput),
      obstacles(obstaclesInput) {
    if (!cursorFont.openFromFile(cursorFontPath)) {
        throw std::runtime_error(std::string("Failed to load cursor font: ") + cursorFontPath);
    }
}

int Map::getElapsedSeconds() const {
    const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::steady_clock::now() - startTime).count();
    return static_cast<int>(std::min(
        elapsed,
        static_cast<decltype(elapsed)>(std::numeric_limits<int>::max())));
}

float Map::getElapsedTimeSeconds() const {
    return std::chrono::duration<float>(
        std::chrono::steady_clock::now() - startTime).count();
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

void Map::updatePassMaskAt(float robotRadius, cords cordsInput){
    float minDistance = robotRadius + squareSide;
    int minSquares = int(minDistance) / squareSide + 2;
    int row = cordsInput.row;
    int col = cordsInput.col;

    if (!isMaskSet(obstacleMask[row][col])){
        for (int drow = -minSquares + 1; drow < minSquares; drow++){
            for (int dcol = -minSquares + 1 ; dcol < minSquares; dcol++){
                if (cordsInMask(obstacleMask, cords{row+drow, col+dcol}))
                    passMask[row + drow][col + dcol] = 0;
            }
        }
    }
}


void Map::updatePassMask(float robotRadius){
    passMaskOnes();
    float minDistance = robotRadius + squareSide;
    int minSquares = int(minDistance) / squareSide + 2;

    for (int row = 0; row < obstacleMask.size(); row ++){
        for (int col = 0; col < obstacleMask[0].size(); col ++){

            if (!isMaskSet(obstacleMask[row][col])){
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

void Map::drawRedZone(sf::RenderWindow& window){
    for (auto& square : obstacleRects) window.draw(square);
}


void Map::setMiniMap(sf::Vector2f topLeft, sf::Vector2f size){
    miniMapBG.setSize(size);
    miniMapBG.setPosition(topLeft);
    miniMapBG.setFillColor(sf::Color::Black);
    miniMapBG.setOutlineColor(sf::Color::White);
    miniMapBG.setOutlineThickness(3);
}

void Map::setObstMaskAt(cords cords, int value){
    if (0 <= cords.row && cords.row < obstacleMask.size() && 0 <= cords.col && cords.col < obstacleMask[0].size())
        obstacleMask[cords.row][cords.col] = value;
    else printf("setObstMaskAt(): wrong cords given\n");
}

void Map::markObstacleFound(cords cords){
    if (cordsInMask(obstacleMask, cords) && isMaskSet(obstacleMask[cords.row][cords.col])) {
        obstacleMask[cords.row][cords.col] = -(getElapsedSeconds() + 1);
    }
    else if (!cordsInMask(obstacleMask, cords)) {
        printf("markObstacleFound(): wrong cords given\n");
    }
}

void Map::setPassMaskAt(cords cords, int value){
    if (cordsInMask(passMask, cords))
        passMask[cords.row][cords.col] = value;
    else printf("setPassMaskAt(): wrong cords given\n");
}

void Map::drawObstaclesMiniMap(sf::RenderWindow& window){

    int factor = mapSize.x / miniMapBG.getSize().x; // must always be integer number
    int squareSideMini = squareSide / factor;
    const float currentTimeSeconds = getElapsedTimeSeconds();

    for (int row = 0; row < obstacleMask.size(); row++){
        for (int col = 0; col < obstacleMask[0].size(); col++){
            if (!isMaskSet(obstacleMask[row][col])){
                sf::RectangleShape square({squareSideMini, squareSideMini});
                square.setPosition({miniMapBG.getPosition().x + col * squareSideMini, miniMapBG.getPosition().y + row * squareSideMini});
                square.setFillColor(obstacleAgeColor(obstacleMask[row][col], currentTimeSeconds));
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
            if (!isMaskSet(passMask[row][col])){
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
    //drawPassMaskMiniMap(window);
    drawObstaclesMiniMap(window);
}

void Map::makeAvoidanceRects(){
    for (unsigned int i = 0; i < passMask.size(); i++){
        auto row = passMask[i];
        for (unsigned j = 0; j < row.size(); j++){
            bool isSquareOk = isMaskSet(row[j]);

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

void Map::drawCursor(sf::RenderWindow& window, const sf::Image& windowCopy, sf::Vector2f leftTop, sf::Color colorToAdd){
    if (squareSide <= 0) return;

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

    const cords gridCords = posToCords(leftTop, squareSide);
    sf::Text coordinates(
        cursorFont,
        "(" + std::to_string(gridCords.row) + ", " + std::to_string(gridCords.col) + ")",
        14);
    coordinates.setFillColor(sf::Color::White);
    coordinates.setOutlineColor(sf::Color::Black);
    coordinates.setOutlineThickness(1.f);

    const sf::Vector2f textSize = coordinates.getLocalBounds().size;
    sf::Vector2f textPosition{leftTop.x + squareSide + 4.f, leftTop.y};
    if (textPosition.x + textSize.x > window.getSize().x) {
        textPosition.x = leftTop.x - textSize.x - 4.f;
    }
    if (textPosition.y + textSize.y > window.getSize().y) {
        textPosition.y = leftTop.y + squareSide - textSize.y;
    }
    if (textPosition.x < 0.f) textPosition.x = 0.f;
    if (textPosition.y < 0.f) textPosition.y = 0.f;

    coordinates.setPosition(textPosition);
    window.draw(coordinates);
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

bool Map::getObstMaskAt(cords cords) const{
    if (0 <= cords.row && cords.row < obstacleMask.size() && 0 <= cords.col && cords.col < obstacleMask[0].size())
        return isMaskSet(obstacleMask[cords.row][cords.col]);
    printf("getObstMaskAt(): wrong cords given\n");
    return 0;
}

bool Map::getPassMaskAt(cords cords) const{
    if (cordsInMask(passMask, cords))
        return isMaskSet(passMask[cords.row][cords.col]);
    printf("getPassMaskAt(): wrong cords given\n");
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
