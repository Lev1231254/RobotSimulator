#include <draw.hpp>

void draw_robot(sf::RenderWindow& window, Robot robot){
        window.draw(robot.getBody());
}