#ifndef APP_H
#define APP_H

#include <SFML/Graphics.hpp>
#include <optional>
#include <objects.hpp>
#include <draw.hpp>
#include <utils.hpp>
#include <iostream>
#include <string>
#include <vector>

float const pi = 3.141592653f;
float const movement_speed = 2.f;
float const turning_speed = pi / 90.f;
float const dTime = 1.f / 90.f;
int const square_side = 16; // side-size of small squares in a grid
int const window_width = 1000;
int const window_height = 1000;
sf::Vector2f const window_size = {window_width, window_height};


class App{
    sf::RenderWindow window;
    Robot robot;
    Map map;
    sf::Image mapImage;

    public:
        App(Robot robotInput) 
            : robot(robotInput){};
        void setRobot(sf::Vector2f position, float radius, float angleRad);
        void setWindow(unsigned int width, unsigned int height, std::string title);
        void addObstacle(Obstacle obstacle);
        void setMapImage();
        void readMap(Map mapInput);
        void run();
};

#endif
