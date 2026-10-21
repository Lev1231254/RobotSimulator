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

void App::scanAndMove(Robot& robot, Map& map, float movement_speed, float turning_speed){

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)){
        sf::Vector2f newPos = robot.getFuturePos(1, movement_speed);

        Robot futureRobot = robot;
        futureRobot.setPosition(newPos);
        
        if (!futureRobot.collidesObsts(map.getObstacles())) robot.moveRobot(1, movement_speed);
    }
        
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)){
        sf::Vector2f newPos = robot.getFuturePos(0, movement_speed);

        Robot futureRobot = robot;
        futureRobot.setPosition(newPos);
        
        if (!futureRobot.collidesObsts(map.getObstacles())) robot.moveRobot(0, movement_speed);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)){
        robot.setAngleRad(robot.getAngleRad() + turning_speed);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)){
        robot.setAngleRad(robot.getAngleRad() - turning_speed);
    }    
}   

void App::runSimulationStep(){
    while (accumulator >= dTime) {
        if (simulationMode == 0){
            scanAndMove(robot, map, movement_speed, turning_speed);
            
        }
        else if (simulationMode == 1){
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            selectedSquare = getSquareInGrid(square_side, {float(mousePos.x), float(mousePos.y)});
        }
        
        accumulator -= dTime;

    }

}

void App::handleEvents(sf::Event event){
    if (event.is<sf::Event::Closed>()){
        window.close();
    }
    
    // toggle pathfinding mode using LeftSHIFT
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()){
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

    // find the best path
    if (event.is<sf::Event::MouseButtonPressed>() && simulationMode == 1){
        int colMouse = int(selectedSquare.x / square_side);
        int rowMouse = int(selectedSquare.y / square_side);
        cords end = {rowMouse, colMouse};

        if (map.getPassMask()[rowMouse][colMouse] != 0){
            sf::Vector2f robotPos = robot.getBodyCircle().getPosition();
            cords start = realPosToMatrix(
                getSquareInGrid(map.getSquareSide(), {robotPos.x, robotPos.y}), 
                square_side
            );

            currentPath.setNodes(findPathAStar(map.getPassMask(), start, end));
        }

        else printf("Robot can't go there");
    }
}

void App::run(){
    map.computePassMask(robot.getRadius());
    map.computeObstacleMask();
    map.makeAvoidanceRects();

    while (window.isOpen()){
        // change modes
        while (const std::optional event = window.pollEvent()){

            handleEvents(*event);

        }
            
        // simulation
        float frameTime = clock.restart().asSeconds();
        accumulator += frameTime;
        runSimulationStep();
    
        // drawing
        window.clear(sf::Color::Black);

        robot.drawRobot(window);
        map.drawObstacles(window);

        if (simulationMode == 1){
            map.addColorToSquare(window, mapImage, selectedSquare, sf::Color::Green);
            map.drawAvoidanceField(window, mapImage);
            if (!currentPath.getNodes().empty()) {
                currentPath.drawPath(window, square_side);
            }
        }
        
        

        window.display();
    }
}