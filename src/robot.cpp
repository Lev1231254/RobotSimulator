#include <robot.hpp>
#include <math.h>


sf::Vector2f Robot::getPosition(){
    return body.getPosition();
}

float Robot::getRadius() {
    return body.getRadius();
}

sf::CircleShape Robot::getBody() {
    return body;
}



void Robot::setPosition(sf::Vector2f position){
    body.setPosition(position);
}

void Robot::setRadius(float radius){
    body.setRadius(radius);
}

void Robot::moveRobot(float angleDeg, float speedPxs){
    //changes position of the robot using angle in degrees and speed in pixel pro second
    float xSpeed = sin(angleDeg) * speedPxs;
    float ySpeed = cos(angleDeg) * speedPxs;

    float x = body.getPosition().x + xSpeed;
    float y = body.getPosition().y + ySpeed;

    body.setPosition({x, y});
}