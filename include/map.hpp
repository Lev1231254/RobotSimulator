#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <obstacle.hpp>
#include <structs.hpp>
#include <utils.hpp>


class Map {
private:
    sf::Vector2u mapSize;
    int squareSide;
    std::vector<Obstacle> obstacles;
    mask2 passMask;
    mask2 obstacleMask;
    std::vector<sf::RectangleShape> obstacleRects;
    sf::RectangleShape miniMapBG;
    sf::Font cursorFont;


public:
    Map(sf::Vector2u mapSizeInput = {0, 0}, int squareSideInput = 0, std::vector<Obstacle> obstaclesInput = {});

    void drawObstacles(sf::RenderWindow& window); //draw using <vector> obstacles
    void drawAvoidanceField(sf::RenderWindow& window);
    void drawCursor(sf::RenderWindow& window, const sf::Image& windowCopy, sf::Vector2f leftTop, sf::Color colorToAdd);
    void drawRobotMiniMap(sf::RenderWindow& window); 
    void drawObstaclesMiniMap(sf::RenderWindow& window); //draw using mask2 obstacleMask
    void drawPassMaskMiniMap(sf::RenderWindow& window);
    void drawMiniMap(sf::RenderWindow& window);


    void setMiniMap(sf::Vector2f topLeft, sf::Vector2f size);
    void setObstMaskAt(cords cords, bool value);
    void setPassMaskAt(cords cords, bool value);
    void addObstacle(Obstacle obstacle);

    void passMaskOnes();
    void setupPassMask(float robotRadius);
    void updatePassMask(float robotRadius);
    void updatePassMaskAt(float robotRadius, cords cords);
    void setupObstacleMask();
    void makeAvoidanceRects();


    std::vector<Obstacle> getObstacles();
    const mask2& getPassMask() const;
    const mask2& getObstacleMask() const;
    bool getObstMaskAt(cords cords) const;
    bool getPassMaskAt(cords cords) const;
    int getSquareSide();
    int getSquareSide() const;
    sf::Vector2u getMapSize();
    const sf::RectangleShape& getMiniMap() const;

    bool isOnMap(sf::Vector2f pos);

};


class MiniMap : public Map{
    Map map;
    int squareSide = 4;
    sf::Vector2u topLeft = {0, 1980 - 1980 / 4};
    sf::Vector2u mapSize = {1980 / 4, 1024 / 4};

    public:
        MiniMap();
        void drawObstacles(sf::RenderWindow& window);
        void drawMap(sf::RenderWindow& window);
};

#endif