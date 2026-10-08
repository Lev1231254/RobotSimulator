#ifndef ASTAR_HPP
#define ASTAR_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include <iostream>
#include <utils.hpp>
#include <nodes.hpp>


Node findClosestNode(Nodes &list);
bool isInMatrix(Pair pos, int lenX, int lenY);
sf::Vector2f pairToVector2f(Pair pair);
Nodes generateSuccessors(std::vector<std::vector<bool>> &avoidanceMatrix, Node &parent, Pair destinationPos);
int nodeInList(Nodes &list, Node &node);
Nodes makePath(Nodes &closedList, Pair startPos, Pair destinationPos);
Nodes findPathAStar(std::vector<std::vector<bool>> avoidanceMatrix, Pair startPos, Pair destinationPos);
void printNodes(Nodes list);
void testAStar();



#endif