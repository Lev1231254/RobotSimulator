#include <objects.hpp>
#include <math.h>


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