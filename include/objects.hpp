#ifndef OBJECTS_H
#define OBJECTS_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <math.h>
#include <structs.hpp>
#include <rayTracer.hpp>
#include <utils.hpp>



class Obstacle {
    sf::RectangleShape body;

    public:
        Obstacle(sf::Vector2f pos, sf::Vector2f size, sf::Color color);
        sf::RectangleShape getBodyRect();
        sf::Vector2f getPosition() const;
        float getWidth() const;
        float getHeight() const;

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
        mask2 obstacleMask;
        std::vector<sf::RectangleShape> obstacleRects;


    public:
        Map(sf::Vector2u mapSizeInput={0,0}, int squareSideInput=0, std::vector<Obstacle> obstaclesInput={})
            : mapSize(mapSizeInput),
            squareSide(squareSideInput),
            obstacles(obstaclesInput) {}; 

        void drawObstacles(sf::RenderWindow& window);
        void drawAvoidanceField(sf::RenderWindow& window, sf::Image windowCopy);
        void addColorToSquare(sf::RenderWindow & window, sf::Image windowCopy, sf::Vector2f leftTop, sf::Color colorToAdd);

        void computePassMask(float robotRadius);
        void computeObstacleMask();
        void makeAvoidanceRects();
        
        void addObstacle(Obstacle obstacle);

        std::vector<Obstacle> getObstacles();
        const mask2& getPassMask() const;
        const mask2& getObstacleMask() const;
        int getSquareSide();
        sf::Vector2u getMapSize();
        bool isOnMap(sf::Vector2f pos);


        bool isLineTraversable(sf::Vector2f start, sf::Vector2f end);
};


class SensorDist {
    ray rayData;
    sf::Vector2f end;
    Map map;
    RayTracer tracer;
    sf::Color color = sf::Color::Red;
    float radius = 10;
    float lineThickness = 5;

    void updateDirection();

public:
    SensorDist(
        ray rayInput,
        const mask2& occupancyMaskInput,
        Map& mapInput,
        bool outsideMaskIsFree = false);

    ray getRay();
    sf::Vector2f getStart();
    sf::Vector2f getDirection();
    sf::Angle getAngle();

    void setStart(sf::Vector2f start);
    void setAngle(sf::Angle angle);
    void setRay(ray rayInput);
    void setPosition(sf::Vector2f pos);

    void updateEnd();
    void drawSensor(sf::RenderWindow& window);
};

class Path {
    Nodes nodes;
    Map &map;
    RayTracer tracer;
    int lineHeight = 6;
    sf::Color color = sf::Color(0, 0, 255, 100);

    public:
        Path(Nodes nodesInput, Map& mapInput);
        
        Nodes getNodes();

        void setNodes(Nodes nodesInput);

        void drawPath(sf::RenderWindow& window, int squareSide);
        void drawLine(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end);

        bool isLineBlocked(Node start, Node end);
        void deleteNode(int k);
        void deleteNodes(int start, int end);
        void smoothPath();
};


class Robot {
    sf::CircleShape body;
    float angleRad;
    SensorDist sensorDist;
private:
    float getAngleToPointf(sf::Vector2f start, sf::Vector2f end);
public:
    Robot(sf::Vector2f pos, float radius, SensorDist sensorDistInput);
    sf::CircleShape getBodyCircle();
    sf::Vector2f getPosition();
    float getAngleRad();
    float getRadius();

    void setPosition(sf::Vector2f position);
    void setRadius(float radius);
    void setAngleRad(float angleRadInput);

    void moveRobot(bool direction, float speedPxs);
    sf::Vector2f getFuturePos(bool direction, float speedPxs);
    bool collidesObst(Obstacle obstacle);
    bool collidesObsts(std::vector<Obstacle> obstacles);

    void traversePath(Path& path);
    void drawRobot(sf::RenderWindow& window);
};


#endif
