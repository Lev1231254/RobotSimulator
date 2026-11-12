#include <aStar.hpp>

Node findClosestNode(Nodes& list){
    Node bestNode = list[0];

    for (Node node : list){
        if (node.f < bestNode.f) bestNode = node;
    }

    return bestNode;
}

Node& findClosestNodeToCords(Nodes& list, cords cords){
    Node& bestNode = list[0];
    float shortestDistance = 999999;

    for (Node& node : list){
        if (getDistance(node.pos, cords) < shortestDistance) {
            bestNode = node;
            shortestDistance = getDistance(node.pos, cords);
        }
    }

    return bestNode;
}

bool isInMatrix(cords pos, int lenX, int lenY){
    return 0 <= pos.col && pos.col < lenX && 0 <= pos.row && pos.row < lenY;
}

sf::Vector2f cordsToVector2f(cords position){
    return {float(position.col), float(position.row)};
}

Node generateNextNode(const mask2& passMask, Node& parent, cords& nextNodeCords, cords& endCords){
    Node successor;
    successor.pos = nextNodeCords;

    bool successorInMatrix = isInMatrix(successor.pos, passMask[0].size(), passMask.size());

    if (successorInMatrix && isMaskSet(passMask[successor.pos.row][successor.pos.col])){
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

Nodes generateSuccessors(const mask2& passMask, Node& parent, cords& destinationCords){
    Nodes successors;

    for (int dcol = -1; dcol< 2; dcol++){
        for (int drow = -1; drow < 2; drow++){
            if (drow == 0 && dcol == 0) continue;

            Node successor;
            successor.pos = {parent.pos.row + drow, parent.pos.col + dcol};

            bool successorInMatrix = isInMatrix(successor.pos, passMask[0].size(), passMask.size());

            if (successorInMatrix && isMaskSet(passMask[successor.pos.row][successor.pos.col])){
                successor.parentPos = parent.pos;
                float g = getDistance(cordsToVector2f(successor.pos), cordsToVector2f(parent.pos)) + parent.g;
                float h = getDistance(cordsToVector2f(successor.pos), cordsToVector2f(destinationCords));
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

Nodes makePath(Nodes& closedList, cords startCords, cords destinationCords){
    Nodes path;

    cords childPos = destinationCords;

    for (std::size_t steps = 0; !(childPos == startCords); steps++){
        if (steps >= closedList.size()) return {};

        bool foundParent = false;
        for (Node& node : closedList){
            if (node.pos == childPos){
                path.push_back(node);
                childPos = node.parentPos;
                foundParent = true;
                break;
            }
        }
        if (!foundParent) return {};
    }

    for (Node& node : closedList){
        if (node.pos == startCords){
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
                if (cordsInMask(passMask, cords) && isMaskSet(passMask[cords.row][cords.col])) return cords;
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

PathStart selectPathStart(const mask2& passMask, cords startCords){
    // 
    if (cordsInMask(passMask, startCords) && isMaskSet(passMask[startCords.row][startCords.col])) {
        return {startCords, true};
    }


    cords freeCords = searchFree(passMask, startCords);
    if (freeCords == nullCords) return {startCords, false};

    bool nearFree = (
        startCords.row - 1 <= freeCords.row && freeCords.row <= startCords.row + 1 &&
        startCords.col - 1 <= freeCords.col && freeCords.col <= startCords.col + 1);
    if (nearFree) return {freeCords, false};

    return {freeCords, true};
}
}

Nodes findPathAStar(const mask2& passMask, cords startCords, cords destinationCords, ChatLog& chatLog){
    PathStart selectedStart = selectPathStart(passMask, startCords);
    cords pathstartCords = selectedStart.position;

    if (selectedStart.selectedFreeSquare && pathstartCords == destinationCords) {
        Node freeStartNode{};
        freeStartNode.pos = pathstartCords;
        freeStartNode.parentPos = pathstartCords;
        return {freeStartNode};
    }

    bool destinationIsUnreachable = 1;

    int c = 0;
    Nodes openList = {};
    Nodes closedList = {};

    Node freeStartNode;
    freeStartNode.pos = pathstartCords;
    freeStartNode.parentPos = pathstartCords;
    freeStartNode.f = 0;
    freeStartNode.g = 0;
    openList.push_back(freeStartNode);

    while (!openList.empty()){
        Node q = findClosestNode(openList);
        openList.erase(find(openList.begin(), openList.end(), q));

        Nodes successors = generateSuccessors(passMask, q, destinationCords);

        for (Node& successor : successors){
            if (successor.pos == destinationCords) {
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
        if (c >= 4000) break;
        closedList.push_back(q);
    }

    if (destinationIsUnreachable) {
        chatLog.addMessage("Unreachable destination");
        Node closestToDestination = findClosestNodeToCords(closedList, destinationCords);
        destinationCords = closestToDestination.pos;
    }


    Nodes path = makePath(closedList, pathstartCords, destinationCords);

    // if the start cords are near free space, build the path from the free space and replace free space with initial cords
    if (selectedStart.selectedFreeSquare == 0){
        Node startNode = {startCords, startCords, 0, 0};
        path.pop_back();
        path.push_back(startNode);
    }
    chatLog.addMessage("Processed total of " + std::to_string(c) + " nodes");
    return path;
}

/*
void printNodes(Nodes list){
    std::cout << std::endl;

    for (Node node : list){
        std::cout << "(" << node.pos.row << ", " << node.pos.col << ") : "
                  << std::fixed << std::setprecision(2) << node.f << '\n';
    }

    std::cout << std::endl;
}

void testAStar(){
    mask2 passMask = {
        {1, 1, 1, 1},
        {1, 0, 0, 1},
        {1, 0, 0, 1},
        {1, 1, 1, 1}
    };

    cords startCords = {0, 0};
    cords destinationCords = {3, 3};

    printNodes(findPathAStar(passMask, startCords, destinationCords));
}
*/
