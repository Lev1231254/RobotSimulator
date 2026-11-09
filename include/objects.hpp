#ifndef OBJECTS_H
#define OBJECTS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <chat.hpp>
#include <structs.hpp>
#include <obstacle.hpp>
#include <map.hpp>
#include <rayTracer.hpp>
#include <utils.hpp>

class SensorDist {
    ray rayData;
    sf::Vector2f end;
    cords endCords;
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
    sf::Vector2f getEndPos() const;
    cords getEndCords() const;
    sf::Vector2f getDirection();
    sf::Angle getAngle() const;

    void setStart(sf::Vector2f start);
    void setAngle(sf::Angle angle);
    void setRay(ray rayInput);
    void setPosition(sf::Vector2f pos);

    void updateEnd();
    void drawSensor(sf::RenderWindow& window);
};

class Path {
    Nodes nodes;
    int traversedNodes = 0;
    Map& map;
    RayTracer tracer;
    int lineHeight = 6;
    sf::Color color = sf::Color(0, 0, 255, 100);

    public:
        Path(Nodes nodesInput, Map& mapInput);
        
        const Nodes& getNodes() const;
        const Map& getMap() const;
        const int getTraversedNodes() const;
        sf::Vector2f getEndPos() const;
        cords getEndCords() const;

        void setNodes(Nodes nodesInput, ChatLog& chatLog);
        void setTraversedNodes(int num);
        void traversedNodesInc();

        void drawPath(sf::RenderWindow& window, int squareSide);
        void drawLine(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end);

        bool isLineBlocked(Node start, Node end);
        void deleteNode(int k);
        void deleteNodes(int start, int end);
        void smoothPath(ChatLog& chatLog);
};


class Robot {
    sf::CircleShape body;
    sf::Angle angle;
    float movementSpeed = 1;
    sf::Angle rotationSpeed = sf::degrees(1);
    std::vector<SensorDist> sensors;
    Map innerMap;
    ChatLog& chatLog;
    bool foundNewObst = 0;
private:
    void updateSensorAngles();

public:
    Robot(
        sf::Vector2f pos,
        float radius,
        SensorDist sensorDistInput,
        float movementSpeedInput,
        sf::Angle rotationSpeedInput,
        Map innerMapInput,
        ChatLog& chatLogInput);

    void updateInnerMap();
    bool foundNewObstacle() const;

    sf::CircleShape getBodyCircle();
    sf::Vector2f getPosition();
    float getAngleRad();
    float getRadius();
    const Map& getInnerMap() const;
    ChatLog& getChatLog() const;
    

    void setPosition(sf::Vector2f position);
    void setRadius(float radius);
    void setAngleRad(float angleRadInput);
    void setMovementSpeed(float speed);
    void setRotationSpeed(sf::Angle speed);
    void setFoundNewObstacle(bool b);
    
    
    void moveRobot(int direction);
    void moveToTarget(sf::Vector2f target);
    void rotateRobot(int direction);
    void turnToAngle(sf::Angle targetAngle);
    void goToNode(Node node, int squareSide);
    bool isAtNode(Node node, int squareSide);
    void traversePath(Path& path);

    sf::Vector2f getFuturePos(bool direction);
    bool collidesObst(Obstacle obstacle);
    bool collidesObsts(std::vector<Obstacle> obstacles);


    void drawRobotMiniMap(sf::RenderWindow& window);
    void drawRobot(sf::RenderWindow& window);
    void drawLineToPath(sf::RenderWindow &window, Path &path);
    void drawSensors(sf::RenderWindow& window);
    void drawMiniMapSensors(
        sf::RenderWindow& window,
        const sf::RectangleShape& miniMap,
        const sf::CircleShape& miniBody);
    void drawMiniMap(sf::RenderWindow& window);
};


#endif
