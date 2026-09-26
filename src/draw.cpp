#include <draw.hpp>
#include <iostream>

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

void makeSquareGreener(sf::RenderWindow & window, sf::Image windowCopy, sf::Vector2f leftTop, float sideLen){
    int x = leftTop.x;
    int y = leftTop.y;
    
    int xMax = window.getSize().x;
    int yMax = window.getSize().y;

    if (x < 0 || x >= xMax || y < 0 || y >= yMax) return;
    sf::Color color = windowCopy.getPixel({x,y});

    color = color + sf::Color(0, 200, 0, 255);
    color = color - sf::Color(100, 0, 100, 0);

    

    
    
    color.a = 255;

    sf::RectangleShape square({sideLen, sideLen});
    square.setPosition(leftTop);
    square.setFillColor(color);

    window.draw(square);
}