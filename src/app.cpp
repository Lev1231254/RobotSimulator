#include <app.hpp>

#include <cmath>
#include <stdexcept>

namespace {
const char* controlsFontPath = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
}

App::App(sf::RenderWindow& windowInput, Robot robotInput, Map mapInput)
    : window(windowInput),
      robot(robotInput),
      map(mapInput),
      controlsText(controlsFont, "", 30),
      currentPath(Nodes{}, map) {
    if (!controlsFont.openFromFile(controlsFontPath)) {
        throw std::runtime_error(std::string("Failed to load controls font: ") + controlsFontPath);
    }

    controlsText.setString(
        "CONTROLS GUIDE\n\n"
        "LSHIFT  -  switch modes\n"
        "R       -  toggle red zone\n"
        "ESC     -  exit program\n\n"
        "MOVEMENT MODE:\n"
        "  WASD  -  move\n\n"
        "PATHFINDING MODE:\n"
        "  LMouse click  -  choose destination\n\n"
        "Press any key to continue");
    controlsText.setFillColor(sf::Color::White);
    controlsText.setLineSpacing(1.25f);

    controlsBackground.setFillColor(sf::Color(20, 20, 20, 240));
    controlsBackground.setOutlineColor(sf::Color(180, 180, 180));
    controlsBackground.setOutlineThickness(2.f);
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

    if (simulationMode == "CONTROLS_GUIDE"){
        simulationMode = "FREE_MOVEMENT";
        return;
    }

    if (key->scancode == sf::Keyboard::Scancode::LShift){
        if (simulationMode == "PATHFINDING") {
            simulationMode = "FREE_MOVEMENT";
            robot.getChatLog().addMessage("Changed to free movement");
        }
        else if (simulationMode == "FREE_MOVEMENT"){
            robot.getChatLog().addMessage("Changed to pathfinding");
            simulationMode = "PATHFINDING";
        }
        return;
    }

    else if (key->scancode == sf::Keyboard::Scancode::R){

        if (redZoneOn) redZoneOn = 0;
        else redZoneOn = 1;
        return;
    }

    else if (key->scancode == sf::Keyboard::Scancode::Escape){
        window.close();
    }


}

void App::handleMouseButtonPressed(){
    if (simulationMode == "PATHFINDING") {
        int colMouse = int(selectedSquare.x / square_side);
        int rowMouse = int(selectedSquare.y / square_side);
        cords end = {rowMouse, colMouse};

        if (isMaskSet(robot.getInnerMap().getPassMask()[rowMouse][colMouse])){
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

void App::handleEvents(sf::Event event){
    if (event.is<sf::Event::Closed>()){
        window.close();
    }

    // toggle pathfinding mode using LeftSHIFT
    if (const auto* key = event.getIf<sf::Event::KeyPressed>()){
        handleKeyPressed(key);
    }

    // find the best path
    if (event.is<sf::Event::MouseButtonPressed>()){
        handleMouseButtonPressed();
    }
}

void App::drawAll(){

    if (simulationMode == "CONTROLS_GUIDE"){
        drawControlsWindow();
        return;
    }

    if (redZoneOn) map.drawRedZone(window);

    if (simulationMode == "FREE_MOVEMENT"){
        robot.drawRobot(window);
        map.drawObstacles(window);
        robot.drawMiniMap(window);
        robot.getChatLog().drawChat(window);
    }

    else if (simulationMode == "PATHFINDING"){
        robot.drawRobot(window);
        map.drawObstacles(window);
        robot.drawMiniMap(window);
        robot.getChatLog().drawChat(window);

        map.drawCursor(window, mapImage, selectedSquare, sf::Color::Green);
        if (!currentPath.getNodes().empty()) {
            currentPath.drawPath(window, square_side);
            robot.drawLineToPath(window, currentPath);
        }
    }

}

void App::drawControlsWindow(){
    const sf::Vector2f windowSize(window.getSize());
    const sf::Vector2f panelSize{
        std::min(windowSize.x - 40.f, 700.f),
        std::min(windowSize.y - 40.f, 620.f)
    };
    controlsBackground.setSize(panelSize);
    controlsBackground.setPosition({
        (windowSize.x - panelSize.x) / 2.f,
        (windowSize.y - panelSize.y) / 2.f
    });

    const sf::Vector2f textSize = controlsText.getLocalBounds().size;
    controlsText.setPosition({
        std::round(controlsBackground.getPosition().x + (panelSize.x - textSize.x) / 2.f),
        std::round(controlsBackground.getPosition().y + (panelSize.y - textSize.y) / 2.f)
    });

    window.draw(controlsBackground);
    window.draw(controlsText);
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
        window.clear(sf::Color::Black);
        drawAll();
        window.display();
    }
}