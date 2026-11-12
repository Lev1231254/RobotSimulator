#include <utils.hpp>



int sign(float num){
    return (num >= 0) ? 1 : -1;
}

float clamp(float value, float min, float max){
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float exponentialTransition(float value, float cap){
    if (cap <= 0.f) return 1.f;

    const float progress = clamp(value / cap, 0.f, 1.f);
    constexpr float curveStrength = 3.5;
    return -std::expm1(-curveStrength * progress)
        / -std::expm1(-curveStrength);
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

float getDistance(cords pos1, cords pos2){
    float colDistance = pos1.col - pos2.col;
    float rowDistance = pos1.row - pos2.row;
    return sqrt(pow(colDistance, 2) + pow(rowDistance, 2));
}

cords posToCords(sf::Vector2f pos, int squareSide){
    return {int(pos.y / squareSide), int(pos.x / squareSide)};
}

sf::Vector2f cordsToPos(cords cords, int squareSide){
    return {cords.col * squareSide, cords.row * squareSide};
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

float angleToPointf(sf::Vector2f start, sf::Vector2f end){
    sf::Vector2f distances = end - start;
    return atan2(distances.y, distances.x);
}

sf::Angle angleToPos(sf::Vector2f start, sf::Vector2f end){
    sf::Vector2f distances = end - start;
    return sf::radians(atan2(distances.y, distances.x));
}


bool anglesEqual(sf::Angle angle1, sf::Angle angle2){
    // the 0.5 degree difference is almost nothing
    if (abs(angle1.asDegrees() - angle2.asDegrees()) < 0.5f) return true;
    return false;
}

void normalizeAngle(sf::Angle& angle){
    float angleDeg = angle.asDegrees();
    normalizeAngleDeg(angleDeg);
    angle = sf::degrees(angleDeg);
}

void normalizeAngleDeg(float& angle){
    while (angle > 180) angle -= 360;
    while (angle < -180) angle += 360;
}

sf::Vector2u vec_f_to_u(sf::Vector2f vec){
    return {u_int(vec.x), u_int(vec.y)};
}

sf::Vector2f vec_u_to_f(sf::Vector2u vec){
    return {float(vec.x), float(vec.y)};
}

sf::Vector2f posToMiniMap(sf::Vector2f pos, sf::Vector2u size, const sf::RectangleShape& miniMap){
    int factor = size.x / miniMap.getSize().x;
    sf::Vector2f miniPos = {pos.x / factor, pos.y / factor};
    miniPos = miniPos + miniMap.getPosition();
    return miniPos;
}

bool cordsInMask(const mask2& mask, cords cords){
    if (mask.size() == 0 || mask[0].size() == 0) return 0;

    int rowMax = mask.size();
    int colMax = mask[0].size();

    return (0 <= cords.row && cords.row < rowMax) && (0 <= cords.col && cords.col < colMax);
}