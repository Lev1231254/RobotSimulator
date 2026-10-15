#ifndef ASTAR_HPP
#define ASTAR_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include <iostream>
#include <utils.hpp>
#include <structs.hpp>


Node findClosestNode(Nodes &list);
bool isInMatrix(cords pos, int lenX, int lenY);
sf::Vector2f cordsToVector2f(cords position);
Nodes generateSuccessors(const std::vector<std::vector<bool>> &avoidanceMatrix, Node &parent, cords &destinationPos);
int nodeInList(Nodes &list, Node &node);
Nodes makePath(Nodes &closedList, cords startPos, cords destinationPos);
Nodes findPathAStar(const std::vector<std::vector<bool>> &avoidanceMatrix, cords &startPos, cords &destinationPos);
void printNodes(Nodes list);
void testAStar();



#endif