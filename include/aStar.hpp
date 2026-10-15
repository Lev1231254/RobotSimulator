#ifndef ASTAR_HPP
#define ASTAR_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include <iostream>
#include <utils.hpp>
#include <nodes.hpp>


Node findClosestNode(Nodes &list);
bool isInMatrix(cords pos, int lenX, int lenY);
sf::Vector2f cordsToVector2f(cords position);
Nodes generateSuccessors(std::vector<std::vector<bool>> &avoidanceMatrix, Node &parent, cords destinationPos);
int nodeInList(Nodes &list, Node &node);
Nodes makePath(Nodes &closedList, cords startPos, cords destinationPos);
Nodes findPathAStar(std::vector<std::vector<bool>> avoidanceMatrix, cords startPos, cords destinationPos);
void printNodes(Nodes list);
void testAStar();



#endif