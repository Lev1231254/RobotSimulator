#include <aStar.hpp>


Node findClosestNode(Nodes& list){
    Node bestNode = list[0];

    for (Node node : list){
        if (node.f < bestNode.f) bestNode = node;
    }

    return bestNode;
}

bool isInMatrix(Pair pos, int lenX, int lenY){
    return 0 <= pos.first && pos.first < lenX && 0 <= pos.second && pos.second < lenY;
}

sf::Vector2f pairToVector2f(Pair pair){
    return {float(pair.first), float(pair.second)};
}

Nodes generateSuccessors(std::vector<std::vector<bool>>& avoidanceMatrix, Node& parent, Pair destinationPos){
    Nodes successors;

    for (int dcol = -1; dcol< 2; dcol++){
        for (int drow = -1; drow < 2; drow++){
            if (drow == 0 && dcol == 0) continue;

            Node successor;
            successor.pos = { parent.pos.first + dcol, parent.pos.second + drow};

            bool successorInMatrix = isInMatrix(successor.pos, avoidanceMatrix[0].size(), avoidanceMatrix.size());

            if (successorInMatrix && avoidanceMatrix[successor.pos.second][successor.pos.first]){
                successor.parentPos = parent.pos;
                float g = getDistance(pairToVector2f(successor.pos), pairToVector2f(parent.pos)) + parent.g;
                float h = getDistance(pairToVector2f(successor.pos), pairToVector2f(destinationPos));
                successor.f = g + h;
                successor.g = g;
                successors.push_back(successor);
            }
            
        }
    }

    return successors;
}


int nodeInList(Nodes& list, Node& node){
    // checks only positions
    // return -1, if not found
    // return index, if found
    for (int i = 0; i < list.size(); i ++){
        Node element = list[i];
        if (element.pos == node.pos) return i;
    }
    return -1;
}

Nodes makePath(Nodes &closedList, Pair startPos, Pair destinationPos){
    Nodes path;

    Pair childPos = destinationPos;

    while (childPos != startPos){
        for (Node &node : closedList){
            if (node.pos == childPos){
                path.push_back(node);
                childPos = node.parentPos;
                break;
            }
        }
    }

    for (Node &node : closedList){
        if (node.pos == startPos){
            path.push_back(node);
            break;
        }
    }

    return path;
}


Nodes findPathAStar(std::vector<std::vector<bool>> avoidanceMatrix, Pair startPos, Pair destinationPos){
    int c = 0;
    Nodes openList = {};
    Nodes closedList = {};

    Node startNode;
    startNode.pos= startPos;
    startNode.parentPos = startPos;
    startNode.f = 0;
    startNode.g = 0;
    openList.push_back(startNode);

    while (!openList.empty()){
        Node q = findClosestNode(openList);
        openList.erase(find(openList.begin(), openList.end(), q));

        Nodes successors = generateSuccessors(avoidanceMatrix, q, destinationPos);

        for (Node &successor : successors){
            if (successor.pos == destinationPos) {
                closedList.push_back(successor);
                openList = {}; // empty the list
                break;
            }
            else{
                // skip the nodes, if the closer nodes are already found
                int iClosed = nodeInList(closedList, successor);
                if (iClosed != -1){
                    continue;
                }

                int iOpen = nodeInList(openList, successor);
                if (iOpen != -1){
                    if (openList[iOpen].f <= successor.f) continue;

                    else {
                        openList[iOpen].parentPos = successor.parentPos;
                        openList[iOpen].f = successor.f;
                    }
                    continue;
                }
                
                openList.push_back(successor);
            }
        }

        c++;
        closedList.push_back(q);
    }

    Nodes path = makePath(closedList, startPos, destinationPos);
    printf("Processed %d nodes\n", c);
    return path;
}

void printNodes(Nodes list){
    std::cout << std::endl;

    for (Node node : list){
        printf("(%d, %d) : %.2f\n", node.pos.first, node.pos.second, node.f);
    }

    std::cout << std::endl;
}

void testAStar(){
    std::vector<std::vector<bool>> avoidanceMatrix = {
        {1, 1, 1, 1},
        {1, 0, 0, 1},
        {1, 0, 0, 1},
        {1, 1, 1, 1}
    };

    Pair startPos = {0, 0};
    Pair destinationPos = {3, 3};

    printNodes(findPathAStar(avoidanceMatrix, startPos, destinationPos));
}


