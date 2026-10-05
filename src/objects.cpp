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

std::vector<std::vector<bool>> Map::computeAvoidanceMatrix(){
    std::vector<std::vector<bool>> avoidanceMatrix;

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

    return avoidanceMatrix;
}


void Map::drawObstacles(sf::RenderWindow& window){
    for (auto obstacle : obstacles){
        window.draw(obstacle.getBodyRect());
    }
}


void Map::addColorToSquare(sf::RenderWindow & window, sf::Image windowCopy, sf::Vector2f leftTop, sf::Color colorToAdd){
    int x = leftTop.x;
    int y = leftTop.y;
    
    int xMax = window.getSize().x;
    int yMax = window.getSize().y;

    if (x < 0 || x >= xMax || y < 0 || y >= yMax) return;
    sf::Color color = windowCopy.getPixel({x,y});

    color = color + colorToAdd;
    color = color - sf::Color(100, 100, 100, 0);
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