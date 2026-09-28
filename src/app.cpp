#include <app.hpp>


void App::setRobot(sf::Vector2f position, float radius, float angleRad){
    robot.setPosition(position);
    robot.setRadius(radius);
    robot.setAngleRad(angleRad);
}

void App::setWindow(unsigned int width, unsigned int height, std::string title){
    window.create(
        sf::VideoMode({width, height}),
        title,
        sf::Style::Close
    );
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
}

void App::setObstacle(sf::Vector2f size, sf::Vector2f position, sf::Color color){
    obstacle.setSize(size);
    obstacle.setPosition(position);
    obstacle.setFillColor(color);
}

void App::setMapImage(){
    window.draw(obstacle);
    mapImage = getWindowImage(window);
}

void App::run(){
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
            
            // toggle pathfinding mode using LeftSHIFT
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
            makeSquareGreener(window, mapImage, selectedSquare, square_side);
        }
        
        

        window.display();
    }
}