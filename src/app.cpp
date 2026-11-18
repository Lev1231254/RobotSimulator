#include <app.hpp>

sf::Color bg_color(45, 45, 45);

App::App(sf::RenderWindow& windowInput, Robot robotInput, Map mapInput)
    :     window(windowInput),
    robot(robotInput),
    map(mapInput),
    currentPath(Nodes{}, map) {
}

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
        if (simulationMode == "FREE_MOVEMENT"){
            scanAndMove(robot, map);
            
        }
        else if (simulationMode == "PATHFINDING"){
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

void App::handleKeyPressed(const sf::Event::KeyPressed* key){
    if (controlsWindowOn) {
        controlsWindowOn = false;
        return;
    }

    if (key->scancode == sf::Keyboard::Scancode::F1){
        controlsWindowOn = !controlsWindowOn;
        return;
    }

    if (key->scancode == sf::Keyboard::Scancode::LShift){
        if (simulationMode == "PATHFINDING") {
            simulationMode = "FREE_MOVEMENT";
            robot.getChatLog().addMessage("Switched to free movement");
        }
        else if (simulationMode == "FREE_MOVEMENT"){
            robot.getChatLog().addMessage("Switched to pathfinding");
            simulationMode = "PATHFINDING";
        }
        return;
    }

    else if (key->scancode == sf::Keyboard::Scancode::R){
        redZoneOn = !redZoneOn;
        robot.getChatLog().addMessage(redZoneOn ? "Red zone enabled" : "Red zone disabled");
        return;
    }

    else if (key->scancode == sf::Keyboard::Scancode::P){
        pathDrawingOn = !pathDrawingOn;
        robot.getChatLog().addMessage(
            pathDrawingOn ? "Path drawing enabled" : "Path drawing disabled");
        return;
    }

    else if (key->scancode == sf::Keyboard::Scancode::M){
        hudOn = !hudOn;
        robot.getChatLog().addMessage(
            hudOn ? "Minimap enabled" : "Minimap disabled");
        return;
    }

    else if (key->scancode == sf::Keyboard::Scancode::Escape){
        window.close();
    }


}

void App::handleMouseButtonPressed(){
    if (controlsWindowOn) {
        controlsWindowOn = false;
        return;
    }

    if (simulationMode == "PATHFINDING") {
        int colMouse = int(selectedSquare.x / square_side);
        int rowMouse = int(selectedSquare.y / square_side);
        cords end = {rowMouse, colMouse};

        //if (isMaskSet(robot.getInnerMap().getPassMask()[rowMouse][colMouse])){
            sf::Vector2f robotPos = robot.getBodyCircle().getPosition();

            cords start = posToCords(
                getSquareInGrid(robot.getInnerMap().getSquareSide(), {robotPos.x, robotPos.y}),
                square_side
            );

            currentPath.setNodes(
                findPathAStar(robot.getInnerMap().getPassMask(), start, end, robot.getChatLog()),
                robot.getChatLog());
        //}

        //else robot.getChatLog().addMessage("Unreachable destination");
    }
}

void App::handleMouseWheelScrolled(const sf::Event::MouseWheelScrolled* wheel) {
    ChatLog& chatLog = robot.getChatLog();
    if (chatLog.isMouseOver(wheel->position)) {
        chatLog.scroll(wheel->delta);
    }
}

void App::handleEvents(sf::Event event){
    if (event.is<sf::Event::Closed>()){
        window.close();
        return;
    }

    // toggle pathfinding mode using LeftSHIFT
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()){
        handleKeyPressed(key);
    }

    if (const auto* wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
        handleMouseWheelScrolled(wheel);
    }

    // find the best path
    if (event.is<sf::Event::MouseButtonPressed>()){
        handleMouseButtonPressed();
    }
}

void App::drawAll(){
    if (redZoneOn) map.drawRedZone(window);

    if (simulationMode == "FREE_MOVEMENT"){
        robot.drawRobot(window);
        map.drawObstacles(window);
        if (hudOn) {
            robot.drawMiniMap(window);
            robot.getChatLog().drawChat(window);
        }
    }

    else if (simulationMode == "PATHFINDING"){
        robot.drawRobot(window);
        map.drawObstacles(window);
        if (hudOn) robot.drawMiniMap(window);

        map.drawCursor(window, mapImage, selectedSquare, sf::Color::Green);
        if (pathDrawingOn && !currentPath.getNodes().empty()) {
            currentPath.drawPath(window, square_side);
            robot.drawLineToPath(window, currentPath);
        }

        if (hudOn) robot.getChatLog().drawChat(window);
    }

    if (controlsWindowOn) controlsGuide.draw(window);
}

void App::run(){
    

    while (window.isOpen()){

        // handle events
        const unsigned int maxEventsPerFrame = 100;
        for (unsigned int eventCount = 0; eventCount < maxEventsPerFrame; ++eventCount){
            const std::optional event = window.pollEvent();
            if (!event) break;
            handleEvents(*event);

        }
            
        // run simulation
        float frameTime = clock.restart().asSeconds();
        accumulate(frameTime);
        runSimulationStep();
    
        // draw
        window.clear(bg_color);
        drawAll();
        window.display();
    }
}