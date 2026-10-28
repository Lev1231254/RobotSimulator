#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <obstacle.hpp>
#include <structs.hpp>

class Map {
private:
    sf::Vector2u mapSize;
    int squareSide;
    std::vector<Obstacle> obstacles;
    mask2 passMask;
    mask2 obstacleMask;
    std::vector<sf::RectangleShape> obstacleRects;

public:
    Map(sf::Vector2u mapSizeInput = {0, 0}, int squareSideInput = 0, std::vector<Obstacle> obstaclesInput = {});

    void drawObstacles(sf::RenderWindow& window);
    void drawAvoidanceField(sf::RenderWindow& window, sf::Image windowCopy);
    void addColorToSquare(sf::RenderWindow& window, sf::Image windowCopy, sf::Vector2f leftTop, sf::Color colorToAdd);

    void computePassMask(float robotRadius);
    void computeObstacleMask();
    void makeAvoidanceRects();

    void addObstacle(Obstacle obstacle);

    std::vector<Obstacle> getObstacles();
    const mask2& getPassMask() const;
    const mask2& getObstacleMask() const;
    int getSquareSide();
    int getSquareSide() const;
    sf::Vector2u getMapSize();
    bool isOnMap(sf::Vector2f pos);

    bool isLineTraversable(sf::Vector2f start, sf::Vector2f end);
};

#endif