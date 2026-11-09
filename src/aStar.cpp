#include <aStar.hpp>
#include <iomanip>

Node findClosestNode(Nodes& list){
    Node bestNode = list[0];

    for (Node node : list){
        if (node.f < bestNode.f) bestNode = node;
    }

    return bestNode;
}

bool isInMatrix(cords pos, int lenX, int lenY){
    return 0 <= pos.col && pos.col < lenX && 0 <= pos.row && pos.row < lenY;
}

sf::Vector2f cordsToVector2f(cords position){
    return {float(position.col), float(position.row)};
}

Node generateNextNode(const std::vector<std::vector<bool>>& avoidanceMatrix, Node& parent, cords &nextNodeCords, cords& endCords){
    Node successor;
    successor.pos = nextNodeCords;

    bool successorInMatrix = isInMatrix(successor.pos, avoidanceMatrix[0].size(), avoidanceMatrix.size());

    if (successorInMatrix && avoidanceMatrix[successor.pos.row][successor.pos.col]){
        successor.parentPos = parent.pos;
        float g = getDistance(cordsToVector2f(successor.pos), cordsToVector2f(parent.pos)) + parent.g;
        float h = getDistance(cordsToVector2f(successor.pos), cordsToVector2f(endCords));
        successor.f = g + h;
        successor.g = g;
        return successor;
    }
    printf("Tried to generate node at false cords\n");
    Node node;
    return node;
}

Nodes generateSuccessors(const std::vector<std::vector<bool>>& avoidanceMatrix, Node& parent, cords &destinationPos){
    Nodes successors;

    for (int dcol = -1; dcol< 2; dcol++){
        for (int drow = -1; drow < 2; drow++){
            if (drow == 0 && dcol == 0) continue;

            Node successor;
            successor.pos = {parent.pos.row + drow, parent.pos.col + dcol};

            bool successorInMatrix = isInMatrix(successor.pos, avoidanceMatrix[0].size(), avoidanceMatrix.size());

            if (successorInMatrix && avoidanceMatrix[successor.pos.row][successor.pos.col]){
                successor.parentPos = parent.pos;
                float g = getDistance(cordsToVector2f(successor.pos), cordsToVector2f(parent.pos)) + parent.g;
                float h = getDistance(cordsToVector2f(successor.pos), cordsToVector2f(destinationPos));
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
    for (std::size_t i = 0; i < list.size(); i ++){
        Node element = list[i];
        if (element.pos == node.pos) return static_cast<int>(i);
    }
    return -1;
}

Nodes makePath(Nodes &closedList, cords startPos, cords destinationPos){
    Nodes path;

    cords childPos = destinationPos;

    for (std::size_t steps = 0; !(childPos == startPos); steps++){
        if (steps >= closedList.size()) return {};

        bool foundParent = false;
        for (Node &node : closedList){
            if (node.pos == childPos){
                path.push_back(node);
                childPos = node.parentPos;
                foundParent = true;
                break;
            }
        }
        if (!foundParent) return {};
    }

    for (Node &node : closedList){
        if (node.pos == startPos){
            path.push_back(node);
            break;
        }
    }  

    

    return path;
}

cords searchFree(const mask2& passMask, cords originCords){
    if (passMask.empty() || passMask[0].empty()) return nullCords;

    const int maxRadius = static_cast<int>(std::max(passMask.size(), passMask[0].size()));
    for (int radius = 1; radius <= maxRadius; radius++){
        for (int drow = -radius; drow < radius + 1; drow++){
            for (int dcol = -radius; dcol < radius + 1; dcol++){
                if (drow != -radius && drow != radius && dcol != -radius && dcol != radius) continue;

                cords cords{originCords.row + drow, originCords.col + dcol};
                if (cordsInMask(passMask, cords) && passMask[cords.row][cords.col] == 1) return cords;
            }
        }
    }
    return nullCords;
}

void addFree(const mask2& passMask, Node& originNode, cords endCords, Nodes& nodesList){
    cords freeCords = searchFree(passMask, originNode.pos);
    if (freeCords == nullCords) return;

    Node freeNode = generateNextNode(passMask, originNode, freeCords, endCords);
    nodesList.push_back(freeNode);
}

namespace {
struct PathStart {
    cords position;
    bool selectedFreeSquare;
};

PathStart selectPathStart(const mask2& passMask, cords startPos){
    // cordsInMask(passMask, startPos) && passMask[startPos.row][startPos.col]
    if (cordsInMask(passMask, startPos)) {
        return {startPos, false};
    }

    cords freeCords = searchFree(passMask, startPos);
    if (freeCords == nullCords) return {startPos, false};

    bool nearFree = (
        startPos.row - 1 <= freeCords.row && freeCords.row <= startPos.row + 1 &&
        startPos.col - 1 <= freeCords.col && freeCords.col <= startPos.col + 1);
    if (nearFree) return {startPos, false};

    return {freeCords, true};
}
}

Nodes findPathAStar(const std::vector<std::vector<bool>> &avoidanceMatrix, cords &startPos, cords &destinationPos, ChatLog* chatLog){
    PathStart selectedStart = selectPathStart(avoidanceMatrix, startPos);
    cords pathStartPos = selectedStart.position;

    if (selectedStart.selectedFreeSquare && pathStartPos == destinationPos) {
        Node startNode{};
        startNode.pos = pathStartPos;
        startNode.parentPos = pathStartPos;
        return {startNode};
    }

    bool destinationIsUnreachable = 1;

    int c = 0;
    Nodes openList = {};
    Nodes closedList = {};

    Node startNode;
    startNode.pos = pathStartPos;
    startNode.parentPos = pathStartPos;
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
                destinationIsUnreachable = 0;
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

    if (destinationIsUnreachable) {
        if (chatLog != nullptr) {
            chatLog->addMessage("Unreachable destination");
        }
        return {};
    }

    Nodes path = makePath(closedList, pathStartPos, destinationPos);
    if (chatLog != nullptr) {
        chatLog->addMessage("Processed total of " + std::to_string(c) + " nodes");
    }
    return path;
}

void printNodes(Nodes list){
    std::cout << std::endl;

    for (Node node : list){
        std::cout << "(" << node.pos.row << ", " << node.pos.col << ") : "
                  << std::fixed << std::setprecision(2) << node.f << '\n';
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

    cords startPos = {0, 0};
    cords destinationPos = {3, 3};

    printNodes(findPathAStar(avoidanceMatrix, startPos, destinationPos));
}
