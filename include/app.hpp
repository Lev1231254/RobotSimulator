#ifndef APP_H
#define APP_H

#include <SFML/Graphics.hpp>
#include <optional>
#include <objects.hpp>
#include <utils.hpp>
#include <string>
#include <vector>
#include <aStar.hpp>


float const pi = 3.141592653f;
float const movement_speed = 2.f;
float const turning_speed = pi / 120.f;
float const dTime = 1.f / 90.f;
float const pathFindingInterval = 0.3f;
int const square_side = 16; // side-size of small squares in a grid
int const window_width = 1000;
int const window_height = 1000;
sf::Vector2f const window_size = {window_width, window_height};


class App{
    sf::RenderWindow& window;
    Robot robot;
    Map map;
    sf::Image mapImage;
    sf::Clock clock;
    sf::Font controlsFont;
    sf::Text controlsText;
    sf::RectangleShape controlsBackground;
    float accumulator = 0.f;
    float pathFindingAccumulator = 0.f;
    sf::Vector2f selectedSquare;
    Path currentPath;

    std::string simulationMode = "CONTROLS_GUIDE";
    bool redZoneOn = 0;
    bool pathDrawingOn = true;
    bool controlsWindowOn = 1;

    private:
        void accumulate(float time);
        void scanAndMove(Robot& robot, Map& map);
        void runSimulationStep();
        void handleEvents(sf::Event event);
        void handleKeyPressed(const sf::Event::KeyPressed* key);
        void handleMouseButtonPressed();
        void drawAll();
        void drawControlsWindow();

    public:
        App(sf::RenderWindow& windowInput, Robot robotInput, Map mapInput);
        
        void run();
        void setRobot(sf::Vector2f position, float radius, float angleRad);
        void setMapImage();
};

#endif
