#include <structs.hpp>

sf::Vector2f nodeToVector2f(Node& node, const int& squareSide){
    return {node.pos.col * squareSide, node.pos.row * squareSide};
}