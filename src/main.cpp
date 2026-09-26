#include <SFML/Graphics.hpp>
#include <numbers>
#include <optional>
#include <robot.hpp>
#include <draw.hpp>
#include <utils.hpp>
#include <iostream>

float const pi = 3.141592653f;
float const movement_speed = 2.f;
float const turning_speed = pi / 90.f;
float const dTime = 1.f / 90.f;
int const square_side = 30; // side-size of small squares in a grid
sf::Vector2f const window_size = {1000, 1000};

int main()
{

    sf::RenderWindow window(
        sf::VideoMode({1000, 1000}),
        "Robot simulator",
        sf::Style::Close
    );
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    sf::RectangleShape obstacle({100, 100});
    obstacle.setPosition({300, 300});
    obstacle.setFillColor(sf::Color::Blue);

    window.draw(obstacle);
    sf::Image windowImage = getWindowImage(window);

    Robot robot;
    robot.setPosition({500.0f, 500.0f});
    robot.setRadius(50.0f);
    robot.setAngleRad(pi / 6);




    sf::Clock clock;
    float accumulator = 0.f;

    int simulationMode = 0;
    // 0 - WASD mode
    // 1 - pathfinding mode

    sf::Vector2f selectedSquare;


    while (window.isOpen()){
        while (const std::optional event = window.pollEvent()){

            if (event->is<sf::Event::Closed>()){
                window.close();
            }
            
            // change between modes using LeftSHIFT
            if (const auto* key = event->getIf<sf::Event::KeyPressed>()){
                if (key->scancode == sf::Keyboard::Scancode::LShift){
                    if (simulationMode == 1) {
                        simulationMode = 0;
                        std::cout << "Mode - 0" << std::endl;
                    }
                    else if (simulationMode == 0){
                        std::cout << "Mode - 1" << std::endl;
                        simulationMode = 1;
                    } 
                }
                
            }

        }
        
        // simulation
        float frameTime = clock.restart().asSeconds();
        accumulator += frameTime;
        
        while (accumulator >= dTime) {
            if (simulationMode == 0){
                scanAndMove(robot, obstacle, movement_speed, turning_speed);
                
            }
            else if (simulationMode == 1){
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                selectedSquare = getSquareInGrid(window_size, square_side, mousePos);
            }

            
            
            accumulator -= dTime;

        }

        // drawing
        window.clear(sf::Color::Black);

        draw_robot(window, robot);
        window.draw(obstacle);
        if (simulationMode == 1){
            makeSquareGreener(window, windowImage, selectedSquare, square_side);
        }
        
        

        window.display();
    }

    return 0;
}