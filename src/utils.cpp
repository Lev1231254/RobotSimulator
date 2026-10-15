#include <utils.hpp>


void scanAndMove(Robot& robot, Map map, float movement_speed, float turning_speed){

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)){
        sf::Vector2f newPos = robot.getFuturePos(1, movement_speed);

        Robot futureRobot = robot;
        futureRobot.setPosition(newPos);
        
        if (!robotCollidesObsts(futureRobot, map.getObstacles())) robot.moveRobot(1, movement_speed);
    }
        
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)){
        sf::Vector2f newPos = robot.getFuturePos(0, movement_speed);

        Robot futureRobot = robot;
        futureRobot.setPosition(newPos);
        
        if (!robotCollidesObsts(futureRobot, map.getObstacles())) robot.moveRobot(0, movement_speed);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)){
        robot.setAngleRad(robot.getAngleRad() + turning_speed);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)){
        robot.setAngleRad(robot.getAngleRad() - turning_speed);
    }    
}   


float clamp(float value, float min, float max){
    if (value < min) return min;
    if (value > max) return max;
    return value;
}


bool circleIntersectsRect(sf::CircleShape circle, sf::RectangleShape rect){
    sf::Vector2f rectTopLeft = rect.getPosition();
    sf::Vector2f rectSize = rect.getSize();

    float radius = circle.getRadius();
    sf::Vector2f center = circle.getPosition(); // origin must be set to the center

    float closestX = clamp(center.x, rectTopLeft.x, rectTopLeft.x + rectSize.x);
    float closestY = clamp(center.y, rectTopLeft.y, rectTopLeft.y + rectSize.y);

    float distanceX = center.x - closestX;
    float distanceY = center.y - closestY;

    return distanceX * distanceX + distanceY * distanceY <= radius * radius;
}


bool robotCollidesObst(Robot robot, Obstacle obstacle){
    sf::CircleShape circle = robot.getBodyCircle();
    sf::RectangleShape rect = obstacle.getBodyRect();
    return circleIntersectsRect(circle, rect);
}


bool robotCollidesObsts(Robot robot, std::vector<Obstacle> obstacles){
    for (auto obstacle : obstacles){
        if (robotCollidesObst(robot, obstacle)) return 1;
    }
    return 0;
}


sf::Image getWindowImage(sf::RenderWindow& window){
    sf::Texture texture(window.getSize());
    texture.update(window);

    sf::Image image = texture.copyToImage();
    return image;
}


sf::Vector2f getSquareInGrid(int squareSide, sf::Vector2f position){
    float x = (int(position.x) / squareSide) * squareSide;
    float y = (int(position.y) / squareSide) * squareSide;

    return {x, y};
}

float getDistance(sf::Vector2f pos1, sf::Vector2f pos2){
    float xDistance = pos1.x - pos2.x;
    float yDistance = pos1.y - pos2.y;
    return sqrt( pow(xDistance, 2) + pow(yDistance, 2) );
}

Pair realPosToMatrix(sf::Vector2f pos, int squareSide){
    return {int(pos.x / squareSide), int(pos.y / squareSide)};
}


bool intersectsRectangle(
    const sf::Vector2f& a,
    const sf::Vector2f& b,
    const sf::RectangleShape& rectangle)
{
    const sf::Vector2f position = rectangle.getPosition();
    const sf::Vector2f size = rectangle.getSize();

    const float left   = std::min(position.x, position.x + size.x);
    const float right  = std::max(position.x, position.x + size.x);
    const float top    = std::min(position.y, position.y + size.y);
    const float bottom = std::max(position.y, position.y + size.y);

    float tMin = 0.f;
    float tMax = 1.f;

    auto clipAxis = [&](float origin, float direction,
                        float minimum, float maximum) {
        if (direction == 0.f) {
            return origin >= minimum && origin <= maximum;
        }

        float t1 = (minimum - origin) / direction;
        float t2 = (maximum - origin) / direction;

        if (t1 > t2) {
            std::swap(t1, t2);
        }

        tMin = std::max(tMin, t1);
        tMax = std::min(tMax, t2);

        return tMin <= tMax;
    };

    return clipAxis(a.x, b.x - a.x, left, right)
        && clipAxis(a.y, b.y - a.y, top, bottom);
}


float devideUntilLess(float num, float step){
    int i = 1;
    float currStep = num / i;
    
    
    while(currStep > step){
        currStep = num / i;
        i++;
    }
    
    return currStep;
}


sf::Vector2f normalize(sf::Vector2f vec){
    float sum = getDistance({0, 0}, vec);
    return {vec.x / sum, vec.y / sum};
}