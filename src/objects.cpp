#include <objects.hpp>
#include <math.h>

const float pi = 3.1416;

Robot::Robot(sf::Vector2f pos, float radius){
    body.setPosition(pos);
    body.setRadius(radius);
    angleRad = pi / 2;
    body.setOrigin({radius, radius});
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
}

void Robot::setRadius(float radius){
    body.setRadius(radius);
    body.setOrigin({radius, radius});
}

void Robot::setAngleRad(float angleRadInput){
    angleRad = angleRadInput;
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

sf::Vector2f Obstacle::getPosition(){
    return body.getPosition();
}

float Obstacle::getWidth(){
    return body.getSize().x;
}

float Obstacle::getHeight(){
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

void Map::computeAvoidanceMatrix(){
    for (unsigned int y = 0; y < mapSize.y; y += squareSide){
        avoidanceMatrix.push_back({});
        for (unsigned int x = 0; x < mapSize.x; x += squareSide){
            avoidanceMatrix.back().push_back(1);
            for (auto obstacle : obstacles){
                bool xInRect = obstacle.getPosition().x - squareSide <= x && x <= (obstacle.getPosition().x + obstacle.getWidth());
                bool yInRect = obstacle.getPosition().y - squareSide <= y && y <= (obstacle.getPosition().y + obstacle.getHeight());
                if (xInRect && yInRect) {
                    avoidanceMatrix.back().back() = 0;
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
    for (auto square : avoidanceRects) window.draw(square);
}


void Map::makeAvoidanceRects(){
    for (unsigned int i = 0; i < avoidanceMatrix.size(); i++){
        auto row = avoidanceMatrix[i];
        for (unsigned j = 0; j < row.size(); j++){
            bool isSquareOk = row[j];
            
            if (!isSquareOk) {
                sf::Vector2f squareTopLeft = {j * squareSide, i * squareSide};
                sf::RectangleShape square({squareSide, squareSide});
                square.setPosition(squareTopLeft);
                square.setFillColor(sf::Color(255, 0, 0, 100));
                avoidanceRects.push_back(square);
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

std::vector<std::vector<bool>> Map::getAvoidanceMatrix(){
    return avoidanceMatrix;
}



Nodes Path::getNodes(){
    return nodes;
}

void Path::setNodes(Nodes nodesInput){
    nodes = nodesInput;
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
    line.rotate(sf::radians(atan(yDistance / xDistance)));

    

    line.setPosition(start);
    
    window.draw(line);
}

void Path::drawPath(sf::RenderWindow& window, int squareSide){
    for (int i = 0; i < nodes.size(); i++){
        Node node = nodes[i];
        sf::Vector2f start = {node.parentPos.first * squareSide, node.parentPos.second * squareSide};
        sf::Vector2f end = {node.pos.first * squareSide, node.pos.second * squareSide};

        drawLine(window, start, end);
    }
}