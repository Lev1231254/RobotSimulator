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

Robot::Robot(
    sf::Vector2f pos,
    float radius,
    SensorDist sensorDistInput,
    float movementSpeedInput,
    sf::Angle rotationSpeedInput)
    : angle(sensorDistInput.getAngle()),
      movementSpeed(movementSpeedInput),
      rotationSpeed(rotationSpeedInput),
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
    return angle.asRadians();
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
    angle = sf::radians(angleRadInput);
    normalizeAngle(angle);
    sensorDist.setAngle(angle);
}

void Robot::setMovementSpeed(float speed){
    movementSpeed = speed;
}

void Robot::setRotationSpeed(sf::Angle speed){
    rotationSpeed = speed;
}

void Robot::rotateRobot(int direction){
    angle += rotationSpeed * direction;
    sensorDist.setAngle(angle);
}

void Robot::moveRobot(int direction){

    float xSpeed = direction * cos(angle.asRadians()) * movementSpeed;
    float ySpeed = -1 * direction * sin(angle.asRadians()) * movementSpeed;

    float x = body.getPosition().x + xSpeed;
    float y = body.getPosition().y + ySpeed;

    body.setPosition({x, y});
    sensorDist.setPosition({x, y});
}
void Robot::turnToAngle(sf::Angle targetAngle){
    float angleDeg = angle.asDegrees();
    float targetDeg = targetAngle.asDegrees();

    float diff = targetDeg - angleDeg;
    normalizeAngleDeg(diff);

    int direction = sign(diff);

    // if the difference is small enough, just clips robot to that angle
    if (abs(diff) < rotationSpeed.asDegrees() * 10) {
        angle = targetAngle;
        normalizeAngle(angle);
        sensorDist.setAngle(angle);
    }
    // else turn the robot
    else rotateRobot(direction);
}

void Robot::moveToTarget(sf::Vector2f target){
    float distance = getDistance(body.getPosition(), target);
    
    if (distance == 0) return;
    
    if (distance < movementSpeed) setPosition(target);
    
    else moveRobot(1);
}

void Robot::goToNode(Node node, int squareSide){
    sf::Vector2f targetPosition = cordsToPos(node.pos, squareSide);
    sf::Angle targetAngle = -getAngleToPos(body.getPosition(), targetPosition);
    
    turnToAngle(targetAngle);
    if (anglesEqual(angle, targetAngle)) moveToTarget(targetPosition);
}

bool Robot::isAtNode(Node node, int squareSide){
    sf::Vector2f targetPosition = cordsToPos(node.pos, squareSide);
    //sf::Angle targetAngle = getAngleToPos(body.getPosition(), targetPosition);
    if (getDistance(body.getPosition(), targetPosition) <= squareSide * sqrt(2)) return 1;
    return 0;
}

void Robot::traversePath(Path& path){
    
    if (path.getNodes().size() == 0) return;    
    if (isAtNode(path.getNodes().back(), path.getMap().getSquareSide())) {
        printf("Node %d reached\n", path.getTraversedNodes());
        path.traversedNodesInc();
        path.deleteNode(path.getNodes().size()-1);
    }
    else goToNode(path.getNodes().back(), path.getMap().getSquareSide());

    // if we traversed the last node show the total amount of traversed nodes
    if (path.getNodes().size() == 0) {
        printf("Traversed total of %d nodes\n", path.getTraversedNodes());
        path.setTraversedNodes(0);
    }
}

sf::Vector2f Robot::getFuturePos(bool direction){
    //get future position of the robot after the movement (used to do collisions)
    // direction: 1 - forwards, 0 - backwards
    int direction_factor = 2 * (direction - 0.5);

    float xSpeed = direction_factor * cos(angle.asRadians()) * movementSpeed;
    float ySpeed = -1 * direction_factor * sin(angle.asRadians()) * movementSpeed;

    float x = body.getPosition().x + xSpeed;
    float y = body.getPosition().y + ySpeed;

    return {x, y};
}

bool Robot::collidesObst(Obstacle obstacle){
    sf::CircleShape circle = body;
    sf::RectangleShape rect = obstacle.getBodyRect();
    return circleIntersectsRect(circle, rect);
}


bool Robot::collidesObsts(std::vector<Obstacle> obstacles){
    for (auto obstacle : obstacles){
        if (collidesObst(obstacle)) return 1;
    }
    return 0;
}

void Robot::drawRobot(sf::RenderWindow& window){
    // draw the circle and the direction line

    sf::Vector2f center = body.getPosition();
    float anglePointX = center.x + body.getRadius() * cos(angle.asRadians());
    float anglePointY = center.y - body.getRadius() * sin(angle.asRadians());
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

void Robot::drawLineToPath(sf::RenderWindow &window, Path &path){
    if (path.getNodes().size() == 0) return;

    sf::Vector2f start = body.getPosition();
    sf::Vector2f end = cordsToPos(path.getNodes().back().pos, path.getMap().getSquareSide());
    path.drawLine(window, start, end);
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



// PATH -------------------------------------------------

Path::Path(Nodes nodesInput, Map& mapInput)
    : nodes(nodesInput),
      map(mapInput),
      tracer(map, map.getPassMask()) {
}

const Nodes& Path::getNodes() const{
    return nodes;
}

const int Path::getTraversedNodes() const {
    return traversedNodes;
}

void Path::setTraversedNodes(int num){
    traversedNodes = num;
}
void Path::traversedNodesInc() {
    traversedNodes++;
}

const Map& Path::getMap() const{
    return map;
}

void Path::setNodes(Nodes nodesInput){
    nodes = nodesInput;
    smoothPath();
    traversedNodes = 0;
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

    

    //line.setPosition(start + sf::Vector2f({map.getSquareSide()/2, map.getSquareSide()/2}));
    line.setPosition(start);
    
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
    
    if (nodes.size()==1){
        nodes = {};
        return;
    }
    if (static_cast<std::size_t>(k) == 0) {
        nodes.erase(nodes.begin() + k);
        return;
    }   
    else if (static_cast<std::size_t>(k) < nodes.size() - 1) nodes[k-1].parentPos = nodes[k+1].pos;
    else if (static_cast<std::size_t>(k) == nodes.size() - 1) nodes[k-1].parentPos = nodes[k-1].pos;
    nodes.erase(nodes.begin() + k);
}


void Path::smoothPath(){
    if (nodes.size() < 2) return;

    for (std::size_t i = 0; i + 1 < nodes.size(); i++){
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