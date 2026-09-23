#include <draw.hpp>

void draw_robot(sf::RenderWindow& window, Robot robot){
    // draw the circle and the direction line

    sf::Vector2f center = robot.getPosition();
    float anglePointX = center.x + robot.getRadius() * cos(robot.getAngleRad());
    float anglePointY = center.y - robot.getRadius() * sin(robot.getAngleRad());
    sf::Vector2f anglePoint(anglePointX, anglePointY);

    sf::VertexArray line(sf::PrimitiveType::Lines, 2);
    line[0].position = center;
    line[0].color = sf::Color::Red;
    line[1].position = anglePoint;
    line[1].color = sf::Color::Red;

    window.draw(robot.getBodyCircle());
    window.draw(line);

}