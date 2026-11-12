#include <app.hpp>


void App::setRobot(sf::Vector2f position, float radius, float angleRad){
    robot.setPosition(position);
    robot.setRadius(radius);
    robot.setAngleRad(angleRad);
}

void App::setMapImage(){
    map.drawObstacles(window);
    mapImage = getWindowImage(window);
    

}

void App::scanAndMove(Robot& robot, Map& map){

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)){
        sf::Vector2f oldPos = robot.getPosition();
        sf::Vector2f newPos = robot.getFuturePos(1);

        robot.setPosition(newPos);

        if (robot.collidesObsts(map.getObstacles())) robot.setPosition(oldPos);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)){
        sf::Vector2f oldPos = robot.getPosition();
        sf::Vector2f newPos = robot.getFuturePos(0);

        robot.setPosition(newPos);

        if (robot.collidesObsts(map.getObstacles())) robot.setPosition(oldPos);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)){
        robot.rotateRobot(1);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)){
        robot.rotateRobot(-1);
    }    
}   

void App::runSimulationStep(){
    while (accumulator >= dTime) {
        if (simulationMode == 0){
            scanAndMove(robot, map);
            
        }
        else if (simulationMode == 1){
            // before changing the current path, check the path's existance and new obstacles
            if (!currentPath.getNodes().empty() && robot.foundNewObstacle() &&
                pathFindingAccumulator >= pathFindingInterval){
                cords start = posToCords(robot.getPosition(), map.getSquareSide());
                cords end = currentPath.getEndCords();
                currentPath.setNodes(
                    findPathAStar(robot.getInnerMap().getPassMask(), start, end, robot.getChatLog()),
                    robot.getChatLog());
                pathFindingAccumulator = 0.f;
                robot.setFoundNewObstacle(false);
            }
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            selectedSquare = getSquareInGrid(square_side, {float(mousePos.x), float(mousePos.y)});
            
            robot.updateInnerMap();
            robot.traversePath(currentPath);
        }
        
        accumulator -= dTime;
    }

}

void App::accumulate(float time){
    accumulator += time;
    pathFindingAccumulator += time;
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
                robot.getChatLog().addMessage("Mode - 0");
            }
            else if (simulationMode == 0){
                robot.getChatLog().addMessage("Mode - 1");
                simulationMode = 1;
            } 
        }
        
    }

    // find the best path
    if (event.is<sf::Event::MouseButtonPressed>() && simulationMode == 1){
        int colMouse = int(selectedSquare.x / square_side);
        int rowMouse = int(selectedSquare.y / square_side);
        cords end = {rowMouse, colMouse};

        if (robot.getInnerMap().getPassMask()[rowMouse][colMouse] ==  1){
            sf::Vector2f robotPos = robot.getBodyCircle().getPosition();
            cords start = posToCords(
                getSquareInGrid(robot.getInnerMap().getSquareSide(), {robotPos.x, robotPos.y}),
                square_side
            );

            currentPath.setNodes(
                findPathAStar(robot.getInnerMap().getPassMask(), start, end, robot.getChatLog()),
                robot.getChatLog());
        }

        else robot.getChatLog().addMessage("Unreachable destination");
    }
}

void App::run(){
    

    while (window.isOpen()){
        const unsigned int maxEventsPerFrame = 100;
        for (unsigned int eventCount = 0; eventCount < maxEventsPerFrame; ++eventCount){
            const std::optional event = window.pollEvent();
            if (!event) break;
            handleEvents(*event);

        }
            
        // simulation
        float frameTime = clock.restart().asSeconds();
        accumulate(frameTime);
        runSimulationStep();
    
        // drawing
        window.clear(sf::Color::Black);

        robot.drawRobot(window);
        
        map.drawObstacles(window);

        robot.drawMiniMap(window);

        if (simulationMode == 1){
            map.drawCursor(window, mapImage, selectedSquare, sf::Color::Green);
            map.drawAvoidanceField(window);
            if (!currentPath.getNodes().empty()) {
                currentPath.drawPath(window, square_side);
                robot.drawLineToPath(window, currentPath);
            }
        }
        
        robot.getChatLog().drawChat(window);

        window.display();
    }
}