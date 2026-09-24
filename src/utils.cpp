#include <utils.hpp>
#include <algorithm>
void scanAndMove(Robot& robot, sf::RectangleShape obstacle,float movement_speed, float turning_speed){

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)){
        sf::Vector2f newPos = robot.getFuturePos(1, movement_speed);

        sf::CircleShape oldCircle = robot.getBodyCircle();
        sf::CircleShape newCircle = oldCircle;
        newCircle.setPosition(newPos);
        
        if (!circleIntersectsRect(newCircle, obstacle)) robot.moveRobot(1, movement_speed);
    }
        
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)){
        sf::Vector2f newPos = robot.getFuturePos(0, movement_speed);

        sf::CircleShape oldCircle = robot.getBodyCircle();
        sf::CircleShape newCircle = oldCircle;
        newCircle.setPosition(newPos);
        
        if (!circleIntersectsRect(newCircle, obstacle)) robot.moveRobot(0, movement_speed);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)){
        robot.setAngleRad(robot.getAngleRad() + turning_speed);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)){
        robot.setAngleRad(robot.getAngleRad() - turning_speed);
    }    
}   

float clamp(float value, float min, float max){
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

bool circleIntersectsRect(sf::CircleShape circle, sf::RectangleShape rect){
    sf::Vector2f rectTopLeft = rect.getPosition();
    sf::Vector2f rectSize = rect.getSize();

    float radius = circle.getRadius();
    sf::Vector2f center = circle.getPosition(); // origin must be set to the center

    float closestX = clamp(center.x, rectTopLeft.x, rectTopLeft.x + rectSize.x);
    float closestY = clamp(center.y, rectTopLeft.y, rectTopLeft.y + rectSize.y);

    float distanceX = center.x - closestX;
    float distanceY = center.y - closestY;

    return distanceX * distanceX + distanceY * distanceY <= radius * radius;
}