#include <objects.hpp>
#include <algorithm>
#include <cmath>
#include <math.h>

const float pi = 3.1416;

SensorDist::SensorDist(
    ray rayInput,
    const mask2& occupancyMaskInput,
    Map& mapInput,
    bool outsideMaskIsFree)
    : rayData(rayInput),
      map(mapInput),
      tracer(map, occupancyMaskInput, outsideMaskIsFree) {
    updateDirection();
    updateEnd();
}

sf::Vector2f SensorDist::getStart(){
    return rayData.start;
}

ray SensorDist::getRay(){
    return rayData;
}

sf::Vector2f SensorDist::getDirection(){
    return rayData.direction;
}

sf::Angle SensorDist::getAngle(){
    return rayData.angle;
}

void SensorDist::setAngle(sf::Angle angle){
    rayData.angle = angle;
    updateDirection();
    updateEnd();
}

void SensorDist::setStart(sf::Vector2f start){
    rayData.start = start;
    updateEnd();
}

void SensorDist::setRay(ray rayInput){
    rayData = rayInput;
    updateDirection();
    updateEnd();
}

void SensorDist::updateEnd(){
    end = tracer.getFirstHit(rayData, map);
}

void SensorDist::updateDirection(){
    rayData.direction.x = cos(rayData.angle.asRadians());
    rayData.direction.y = -sin(rayData.angle.asRadians());
}

void SensorDist::drawSensor(sf::RenderWindow & window){
    if (end.x < 0 || end.y < 0) return;

    sf::CircleShape hitMarker(radius);
    hitMarker.setOrigin({radius, radius});
    hitMarker.setFillColor(color);
    hitMarker.setPosition(end);

    sf::VertexArray beam(sf::PrimitiveType::Lines, 2);
    beam[0].position = rayData.start;
    beam[0].color = color;
    beam[1].position = end;
    beam[1].color = color;

    window.draw(beam);
    window.draw(hitMarker);
}

void SensorDist::setPosition(sf::Vector2f pos){
    rayData.start = pos;
    updateEnd();
}


// ROBOT -----------------------------

Robot::Robot(sf::Vector2f pos, float radius, SensorDist sensorDistInput)
    : angleRad(sensorDistInput.getAngle().asRadians()),
      sensorDist(sensorDistInput) {
    body.setPosition(pos);
    body.setRadius(radius);
    body.setOrigin({radius, radius});
    sensorDist.setPosition(pos);
}

sf::Vector2f Robot::getPosition(){
    return body.getPosition();
}

float Robot::getRadius() {
    return body.getRadius();
}

sf::CircleShape Robot::getBodyCircle() {
    return body;
}

float Robot::getAngleRad(){
    return angleRad;
}


void Robot::setPosition(sf::Vector2f position){
    body.setPosition(position);
    sensorDist.setPosition(position);
}

void Robot::setRadius(float radius){
    body.setRadius(radius);
    body.setOrigin({radius, radius});
}

void Robot::setAngleRad(float angleRadInput){
    angleRad = angleRadInput;
    sensorDist.setAngle(sf::radians(angleRadInput));
}

void Robot::moveRobot(bool direction, float speedPxs){
    //moves robot in the direction it's facing
    // 1 - forwards, 0 - backwards
    int direction_factor = 2 * (direction - 0.5);


    float xSpeed = direction_factor * cos(angleRad) * speedPxs;
    float ySpeed = -1 * direction_factor * sin(angleRad) * speedPxs;

    float x = body.getPosition().x + xSpeed;
    float y = body.getPosition().y + ySpeed;

    body.setPosition({x, y});
    sensorDist.setPosition({x, y});
}


sf::Vector2f Robot::getFuturePos(bool direction, float speedPxs){
    //get future position of the robot after the movement (used to do collisions)
    // direction: 1 - forwards, 0 - backwards
    int direction_factor = 2 * (direction - 0.5);


    float xSpeed = direction_factor * cos(angleRad) * speedPxs;
    float ySpeed = -1 * direction_factor * sin(angleRad) * speedPxs;

    float x = body.getPosition().x + xSpeed;
    float y = body.getPosition().y + ySpeed;

    return {x, y};
}

void Robot::drawRobot(sf::RenderWindow& window){
    // draw the circle and the direction line

    sf::Vector2f center = body.getPosition();
    float anglePointX = center.x + body.getRadius() * cos(angleRad);
    float anglePointY = center.y - body.getRadius() * sin(angleRad);
    sf::Vector2f anglePoint(anglePointX, anglePointY);

    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
    line[0].position = center;
    line[0].color = sf::Color::Red;
    line[1].position = anglePoint;
    line[1].color = sf::Color::Red;

    window.draw(body);
    window.draw(line);
    sensorDist.drawSensor(window);
}


// OBSTACLE ------------------------------------------
Obstacle::Obstacle(sf::Vector2f pos, sf::Vector2f size, sf::Color color){
    body.setFillColor(color);
    body.setPosition(pos);
    body.setSize(size);
}

sf::RectangleShape Obstacle::getBodyRect(){
    return body;
}

sf::Vector2f Obstacle::getPosition() const {
    return body.getPosition();
}

float Obstacle::getWidth() const {
    return body.getSize().x;
}

float Obstacle::getHeight() const {
    return body.getSize().y;
}

void Obstacle::setPosition(sf::Vector2f position){
    body.setPosition(position);
}

void Obstacle::setWidth(float width){
    float height = body.getSize().y;
    body.setSize({width, height});
}

void Obstacle::setHeight(float height){
    float width = body.getSize().x;
    body.setSize({width, height});
}

void Obstacle::setSize(sf::Vector2f size){
    body.setSize(size);
}
void Obstacle::setColor(sf::Color color){
    body.setFillColor(color);
}



// MAP -------------------------------

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

sf::Vector2u Map::getMapSize(){
    return mapSize;
}

bool Map::isOnMap(sf::Vector2f pos){
    return (0 <= pos.x && pos.x < mapSize.x) && (0 <= pos.y && pos.y < mapSize.y);
}

// PATH -------------------------------------------------

Path::Path(Nodes nodesInput, Map& mapInput)
    : nodes(nodesInput),
      map(mapInput),
      tracer(map, map.getPassMask()) {
}

Nodes Path::getNodes(){
    return nodes;
}

void Path::setNodes(Nodes nodesInput){
    nodes = nodesInput;
    smoothPath();

}

void Path::drawLine(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end){
    if (end.x < start.x){
        sf::Vector2f startCopy = start;
        start = end;
        end = startCopy;
    }
    float xDistance = end.x - start.x;
    float yDistance = (end.y - start.y);

    sf::RectangleShape line(
        {
            sqrt(pow(xDistance, 2) + pow(yDistance, 2)), 
            lineHeight
        }
    );
    line.setFillColor(color);

    line.setOrigin({0, lineHeight / 2});
    line.rotate(sf::radians(atan2(yDistance, xDistance)));

    

    line.setPosition(start + sf::Vector2f({map.getSquareSide()/2, map.getSquareSide()/2}));
    
    window.draw(line);
}

void Path::drawPath(sf::RenderWindow& window, int squareSide){

    for (unsigned int i = 0; i < nodes.size(); i++){
        Node node = nodes[i];
        sf::Vector2f start = {float(node.parentPos.col * squareSide), float(node.parentPos.row * squareSide)};
        sf::Vector2f end = {float(node.pos.col * squareSide), float(node.pos.row * squareSide)};

        drawLine(window, start, end);
    }
}

bool Path::isLineBlocked(Node start, Node end){
    // initialize ray
    auto startPos = nodeToVector2f(start, map.getSquareSide());
    auto endPos = nodeToVector2f(end, map.getSquareSide());
    sf::Vector2f direction = endPos - startPos;
    sf::Angle angleRad = sf::radians(atan2(direction.y, direction.x));
    ray ray{startPos, direction, angleRad};

    if (tracer.getFirstHitFinite(ray, map, endPos) == sf::Vector2f({-1, -1})) return false;
    return true;
}

void Path::deleteNodes(int start, int end){
    // delete everything between start and end
    int len = end - start - 1;
    while (len > 0){
        deleteNode(start+1);
        len--;
    }
}
void Path::deleteNode(int k){
    if (k < 0 || static_cast<std::size_t>(k) >= nodes.size()) return;
    if (static_cast<std::size_t>(k) < nodes.size() - 1) nodes[k-1].parentPos = nodes[k+1].pos;
    else if (static_cast<std::size_t>(k) == nodes.size() - 1) nodes[k-1].parentPos = nodes[k-1].pos;
    nodes.erase(nodes.begin() + k);
}


void Path::smoothPath(){
    for (std::size_t i = 0; i < nodes.size()-1; i++){
        Node start = nodes[i];
        for (std::size_t j = nodes.size()-1; j >= i + 1; j--){
            Node end = nodes[j];
            if (!isLineBlocked(start, end)){
                deleteNodes(i, j);
                break;
            }
        }

    }
    printf("Path is now smooth\n");
}