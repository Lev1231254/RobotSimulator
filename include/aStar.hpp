#ifndef ASTAR_HPP
#define ASTAR_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include <iostream>
#include <utils.hpp>
#include <structs.hpp>
#include <algorithm>
#include <chat.hpp>


Node findClosestNode(Nodes &list);
bool isInMatrix(cords pos, int lenX, int lenY);
sf::Vector2f cordsToVector2f(cords position);
Node generateNextNode(const mask2& avoidanceMatrix, Node& parent, cords& nextNodeCords, cords& endCords);
Nodes generateSuccessors(const mask2& avoidanceMatrix, Node& parent, cords& destinationPos);
int nodeInList(Nodes &list, Node &node);
Nodes makePath(Nodes &closedList, cords startPos, cords destinationPos);
Nodes findPathAStar(const std::vector<std::vector<bool>> &avoidanceMatrix, cords &startPos, cords &destinationPos, ChatLog* chatLog = nullptr);
void printNodes(Nodes list);
void testAStar();
cords searchFree(const mask2& passMask, cords originCords);
void addFree(const mask2& passMask, Node& originNode, cords endCords, Nodes& nodesList);



#endif