#include <SFML/Graphics.hpp>
#include <optional>
#include <robot.hpp>
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

    // sf::RenderWindow window(
    //     sf::VideoMode({1000, 1000}),
    //     "Robot simulator",
    //     sf::Style::Close
    // );
    // window.setVerticalSyncEnabled(true);
    // window.setKeyRepeatEnabled(false);
    app.setWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    // sf::RectangleShape obstacle({100, 100});
    // obstacle.setPosition({300, 300});
    // obstacle.setFillColor(sf::Color::Blue);
    app.setObstacle(OBSTACLE_SIZE, OBSTACLE_POS, OBSTACLE_COLOR);


    // window.draw(obstacle);
    // sf::Image windowImage = getWindowImage(window);
    app.setMapImage();

    // Robot robot;
    // robot.setPosition({500.0f, 500.0f});
    // robot.setRadius(50.0f);
    // robot.setAngleRad(pi / 6);

    app.setRobot(ROBOT_POS, ROBOT_RADIUS, ROBOT_ANGLE_RAD);

    app.run();

    


    // sf::Clock clock;
    // float accumulator = 0.f;

    // int simulationMode = 0;
    // // 0 - WASD mode
    // // 1 - pathfinding mode

    // sf::Vector2f selectedSquare;


    // while (window.isOpen()){
    //     while (const std::optional event = window.pollEvent()){

    //         if (event->is<sf::Event::Closed>()){
    //             window.close();
    //         }
            
    //         // toggle pathfinding mode using LeftSHIFT
    //         if (const auto* key = event->getIf<sf::Event::KeyPressed>()){
    //             if (key->scancode == sf::Keyboard::Scancode::LShift){
    //                 if (simulationMode == 1) {
    //                     simulationMode = 0;
    //                     std::cout << "Mode - 0" << std::endl;
    //                 }
    //                 else if (simulationMode == 0){
    //                     std::cout << "Mode - 1" << std::endl;
    //                     simulationMode = 1;
    //                 } 
    //             }
                
    //         }

    //     }
        
    //     // simulation
    //     float frameTime = clock.restart().asSeconds();
    //     accumulator += frameTime;
        
    //     while (accumulator >= dTime) {
    //         if (simulationMode == 0){
    //             scanAndMove(robot, obstacle, movement_speed, turning_speed);
                
    //         }
    //         else if (simulationMode == 1){
    //             sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    //             selectedSquare = getSquareInGrid(window_size, square_side, mousePos);
    //         }

            
            
    //         accumulator -= dTime;

    //     }

    //     // drawing
    //     window.clear(sf::Color::Black);

    //     draw_robot(window, robot);
    //     window.draw(obstacle);
    //     if (simulationMode == 1){
    //         makeSquareGreener(window, windowImage, selectedSquare, square_side);
    //     }
        
        

    //     window.display();
    // }

    return 0;
}