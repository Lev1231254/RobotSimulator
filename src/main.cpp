#include <SFML/Graphics.hpp>
#include <optional>
#include <objects.hpp>
#include <draw.hpp>
#include <utils.hpp>
#include <iostream>
#include <app.hpp>
#include <mapData.hpp>

sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

unsigned int const WINDOW_WIDTH = desktop.size.x;
unsigned int const WINDOW_HEIGHT = desktop.size.y;
std::string WINDOW_TITLE = "Robot simulator";

sf::Vector2f const ROBOT_POS = {900, 500};
float const ROBOT_RADIUS = 40;
float const ROBOT_ANGLE_RAD = pi / 6;


int main()
{
    Robot robot(ROBOT_POS, ROBOT_RADIUS);
    App app(robot);

    app.setWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    
    app.readMap(map1);
    app.setMapImage();

    app.run();
    return 0;
}