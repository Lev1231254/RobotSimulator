#ifndef ASTAR_HPP
#define ASTAR_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <objects.hpp>
#include <iostream>
#include <utils.hpp>

typedef std::pair<int, int> Pair;

struct Node {
    Pair pos;
    Pair parentPos;
    float f;

    bool operator==(const Node& other) const {
        return (pos == other.pos) && (parentPos == other.parentPos) && (f == other.f);
    }
};

typedef std::vector<Node> Nodes;


Node findClosestNode(Nodes list);
bool isInMatrix(Pair pos, int lenX, int lenY);
sf::Vector2f pairToVector2f(Pair pair);
Nodes generateSuccessors(std::vector<std::vector<bool>> avoidanceMatrix, Node parent, Pair destinationPos);
int nodeInList(Nodes list, Node node);
Nodes makePath(Nodes closedList, Pair startPos, Pair destinationPos);
Nodes findPathAStar(std::vector<std::vector<bool>> avoidanceMatrix, Pair startPos, Pair destinationPos);
void printNodes(Nodes list);
void testAStar();



#endif