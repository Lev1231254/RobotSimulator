#ifndef OBJECTS_H
#define OBJECTS_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>


class Robot {
    sf::CircleShape body;
    float angleRad;

    public:
        Robot(sf::Vector2f pos, float radius);
        sf::CircleShape getBodyCircle();
        sf::Vector2f getPosition();
        float getAngleRad();
        float getRadius();

        void setPosition(sf::Vector2f position);
        void setRadius(float radius);
        void setAngleRad(float angleRadInput);

        void moveRobot(bool direction, float speedPxs);
        sf::Vector2f getFuturePos(bool direction, float speedPxs);

};

class Obstacle {
    sf::RectangleShape body;

    public:
        Obstacle(sf::Vector2f pos, sf::Vector2f size, sf::Color color);
        sf::RectangleShape getBodyRect();
        sf::Vector2f getPosition();
        float getWidth();
        float getHeight();

        void setPosition(sf::Vector2f position);
        void setWidth(float width);
        void setHeight(float height);
        void setSize(sf::Vector2f size);
        void setColor(sf::Color colorInput);

};
class Map {
    private:
        sf::Vector2u mapSize;
        int squareSide;
        std::vector<Obstacle> obstacles;
        std::vector<std::vector<bool>> avoidanceMatrix;
        std::vector<std::vector<sf::Vector2f>> grid;

    public:
        Map(sf::Vector2u mapSizeInput, int squareSideInput, std::vector<Obstacle> obstaclesInput)
            : mapSize(mapSizeInput),
            squareSide(squareSideInput),
            obstacles(obstaclesInput) {}; 

        void drawObstacles(sf::RenderWindow& window);
        void addColorToSquare(sf::RenderWindow & window, sf::Image windowCopy, sf::Vector2f leftTop, sf::Color colorToAdd);

        std::vector<std::vector<bool>> computeAvoidanceMatrix();
        std::vector<std::vector<sf::Vector2f>> computeGrid();
        
        void addObstacle(Obstacle obstacle);

        std::vector<Obstacle> getObstacles();
        std::vector<std::vector<bool>> getAvoidanceMatrix();
};


#endif
