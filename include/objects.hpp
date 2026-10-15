#ifndef OBJECTS_H
#define OBJECTS_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <math.h>
#include <structs.hpp>
#include <rayTracer.hpp>



class Robot {
    sf::CircleShape body;
    float angleRad;
    SensorDist sensorDist;
    public:
        Robot(sf::Vector2f pos, float radius, SensorDist sensorDist);
        sf::CircleShape getBodyCircle();
        sf::Vector2f getPosition();
        float getAngleRad();
        float getRadius();

        void setPosition(sf::Vector2f position);
        void setRadius(float radius);
        void setAngleRad(float angleRadInput);

        void moveRobot(bool direction, float speedPxs);
        sf::Vector2f getFuturePos(bool direction, float speedPxs);
        void drawRobot(sf::RenderWindow& window);

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
        mask2 passMask;
        std::vector<sf::RectangleShape> obstacleRects;


    public:
        Map(sf::Vector2u mapSizeInput, int squareSideInput, std::vector<Obstacle> obstaclesInput)
            : mapSize(mapSizeInput),
            squareSide(squareSideInput),
            obstacles(obstaclesInput) {}; 

        void drawObstacles(sf::RenderWindow& window);
        void drawAvoidanceField(sf::RenderWindow& window, sf::Image windowCopy);
        void addColorToSquare(sf::RenderWindow & window, sf::Image windowCopy, sf::Vector2f leftTop, sf::Color colorToAdd);

        void computePassMask();
        void makeAvoidanceRects();
        
        void addObstacle(Obstacle obstacle);

        std::vector<Obstacle> getObstacles();
        mask2 getPassMask();
        int getSquareSide();
        sf::Vector2u getMapSize();
        bool isOnMap(sf::Vector2f pos);


        bool isLineTraversable(sf::Vector2f start, sf::Vector2f end);
};


class Path {
    Nodes nodes;
    int lineHeight;
    sf::Color color;

    public:
        Path(Nodes nodesInput = {})
            : nodes(nodesInput),
            lineHeight(6),
            color(sf::Color(0, 0, 255, 100)){};
        
        Nodes getNodes();

        void setNodes(Nodes nodesInput);

        void drawPath(sf::RenderWindow& window, int squareSide);
        void drawLine(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end);

        void smoothPath(std::vector<std::vector<bool>> avoidanceMatrix);
};


class SensorDist{
    ray rayData;
    mask2 occupancyMask;
    sf::Vector2f end;
    Map map;
    RayTracer tracer;
    sf::Color color = sf::Color::Red;
    float radius = 10;
    float lineThickness = 5;
    

    private:
        void updateDirection();

    public:
        SensorDist(ray rayInput, mask2 &occupancyMaskInput, Map &mapInput)
        : rayData(rayInput),
        occupancyMask(occupancyMaskInput),
        map(mapInput),
        tracer(map, occupancyMaskInput) {};

        ray getRay();
        sf::Vector2f getStart();
        sf::Vector2f getDirection();
        sf::Angle getAngle();

        void setStart(sf::Vector2f start);
        void setAngle(sf::Angle angle);
        void setRay(ray ray);
        void setPosition(sf::Vector2f pos);

        void updateEnd();
        void drawSensor(sf::RenderWindow & window);
        

};

#endif
