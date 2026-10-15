#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <SFML/Graphics.hpp>
#include <vector>

struct cords{
    int row;
    int col;

    bool operator==(const cords& other) const {
        return row == other.row && col == other.col;
    }
};

struct Node {
    cords pos;
    cords parentPos;
    float f;
    float g;

    bool operator==(const Node& other) const {
        return (pos == other.pos) && (parentPos == other.parentPos) && (f == other.f);
    }
};

typedef std::vector<Node> Nodes;

struct ray{
    sf::Vector2f start;
    sf::Vector2f direction; //normalized vector
    sf::Angle angle;
};

typedef std::vector<std::vector<bool>> mask2;

#endif