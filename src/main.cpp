#include <SFML/Graphics.hpp>
#include <optional>
#include <objects.hpp>
#include <draw.hpp>
#include <utils.hpp>
#include <iostream>
#include <app.hpp>


unsigned int const WINDOW_WIDTH = 1000;
unsigned int const WINDOW_HEIGHT = 1000;
std::string WINDOW_TITLE = "Robot simulator";

sf::Vector2f const OBSTACLE_SIZE = {100, 100};
sf::Vector2f const OBSTACLE_POS = {300, 300};
sf::Color const OBSTACLE_COLOR = sf::Color::White;


sf::Vector2f const ROBOT_POS = {500, 500};
float const ROBOT_RADIUS = 50;
float const ROBOT_ANGLE_RAD = pi / 6;


int main()
{
    App app;

    app.setWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    Obstacle obst1;
    obst1.setColor(sf::Color::White);
    obst1.setSize({100, 200});
    obst1.setPosition({100, 100});
    app.addObstacle(obst1);
    app.setMapImage();
    app.setRobot(ROBOT_POS, ROBOT_RADIUS, ROBOT_ANGLE_RAD);

    app.run();
    return 0;
}