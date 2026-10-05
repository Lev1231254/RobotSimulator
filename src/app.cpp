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

void App::setMapImage(){
    map.drawObstacles(window);
    mapImage = getWindowImage(window);
    

}

void App::runSimulationStep(){
    std::cout << accumulator << std::endl;
    while (accumulator >= dTime) {
        if (simulationMode == 0){
            std::cout << "Scan and move mode" << std::endl;
            scanAndMove(robot, map, movement_speed, turning_speed);
            
        }
        else if (simulationMode == 1){
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            selectedSquare = getSquareInGrid(square_side, mousePos);
        }

        
        
        accumulator -= dTime;

    }

}

void App::run(){

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
        runSimulationStep();
        
        
        // while (accumulator >= dTime) {
        //     if (simulationMode == 0){
        //         scanAndMove(robot, map, movement_speed, turning_speed);
                
        //     }
        //     else if (simulationMode == 1){
        //         sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        //         selectedSquare = getSquareInGrid(square_side, mousePos);
        //     }

            
            
        //     accumulator -= dTime;

        // }

        // drawing
        window.clear(sf::Color::Black);

        drawRobot(window, robot);
        map.drawObstacles(window);
        if (simulationMode == 1){
            map.addColorToSquare(window, mapImage, selectedSquare, sf::Color::Green);
        }
        
        

        window.display();
    }
}