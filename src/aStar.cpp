#include <aStar.hpp>

std::vector<std::vector<bool>> getAvoidanceMatrix(std::vector<Obstacle> obstacles, int squareSide, sf::Vector2u gridSize){
    std::vector<std::vector<bool>> avoidanceMatrix;

    for (unsigned int y = 0; y < gridSize.y; y += squareSide){
        avoidanceMatrix.push_back({});
        for (unsigned int x = 0; x < gridSize.x; x += squareSide){
            avoidanceMatrix.back().push_back(1);
            for (auto obstacle : obstacles){
                bool xInRect = obstacle.getPosition().x - squareSide <= x && x <= (obstacle.getPosition().x + obstacle.getWidth());
                bool yInRect = obstacle.getPosition().y - squareSide <= y && y <= (obstacle.getPosition().y + obstacle.getHeight());
                if (xInRect && yInRect) {
                    avoidanceMatrix.back().back() = 0;
                    break;
                }
            }
        }
    }

    return avoidanceMatrix;
}