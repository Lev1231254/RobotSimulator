#include <robot.hpp>
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