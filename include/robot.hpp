#ifndef ROBOT_H
#define ROBOT_H

#include <vector>
#include <SFML/Graphics.hpp>

class Robot {;
    sf::CircleShape body;
    float angleRad;

    public:
        sf::CircleShape getBodyCircle();
        sf::Vector2f getPosition();
        float getAngleRad();
        float getRadius();

        void setPosition(sf::Vector2f position);
        void setRadius(float radius);
        void setAngleRad(float angleRadInput);

        void moveRobot(bool direction, float speedPxs);

};

#endif
