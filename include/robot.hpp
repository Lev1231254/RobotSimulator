#ifndef ROBOT_H
#define ROBOT_H

#include <vector>
#include <SFML/Graphics.hpp>

class Robot {;
    sf::CircleShape body;

    public:
        sf::CircleShape getBody();

        sf::Vector2f getPosition();
        float getRadius();

        void setPosition(sf::Vector2f position);
        void setRadius(float radius);

        void moveRobot(float angleDeg, float speedPxs);

};

#endif
