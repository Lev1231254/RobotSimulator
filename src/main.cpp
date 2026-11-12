#include <SFML/Graphics.hpp>
#include <optional>
#include <objects.hpp>
#include <utils.hpp>
#include <iostream>
#include <app.hpp>
#include <mapData.hpp>
#include <cmath>

std::string WINDOW_TITLE = "Robot simulator";


sf::Vector2f const ROBOT_POS = {900, 500};
float const ROBOT_RADIUS = 40;
float const ROBOT_ANGLE_RAD = pi / 6;

int main()
{
    sf::RenderWindow window(
        sf::VideoMode::getDesktopMode(),
        WINDOW_TITLE,
        sf::Style::Close,
        sf::State::Windowed);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    const sf::Vector2u windowSize = window.getSize();
    const sf::Vector2f miniMapSize = {
        windowSize.x / 4.f,
        windowSize.y / 4.f
    };
    constexpr float miniMapMargin = 5.f;
    const sf::Vector2f miniMapPosition = {
        windowSize.x * 3.f / 4.f - miniMapMargin,
        miniMapMargin
    };

    Map map(windowSize, square_side, labyrinthMap);
    Map robotMap(windowSize, square_side, emptyMap);

    map.setupObstacleMask();
    map.setupPassMask(ROBOT_RADIUS);
    map.makeAvoidanceRects();
    //map.setMiniMap(miniMapPosition, miniMapSize);

    robotMap.setupObstacleMask();
    robotMap.setupPassMask(ROBOT_RADIUS);
    robotMap.setMiniMap(miniMapPosition, miniMapSize);


    ray initialRay{};
    initialRay.start = ROBOT_POS;
    initialRay.angle = sf::radians(ROBOT_ANGLE_RAD);
    initialRay.direction = {
        std::cos(ROBOT_ANGLE_RAD),
        -std::sin(ROBOT_ANGLE_RAD)
    };

    SensorDist sensorDist(
        initialRay,
        map.getObstacleMask(),
        map,
        true);
    ChatLog chatLog(
        {
            robotMap.getMiniMap().getPosition().x,
            robotMap.getMiniMap().getPosition().y + robotMap.getMiniMap().getSize().y
        },
        {
            robotMap.getMiniMap().getSize().x,
            160
        });
    Robot robot(
        ROBOT_POS,
        ROBOT_RADIUS,
        sensorDist,
        movement_speed,
        sf::radians(turning_speed),
        robotMap,
        chatLog);
    App app(window, robot, map);
    
    app.setMapImage();

    app.run();
    return 0;
}