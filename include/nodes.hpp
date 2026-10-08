#ifndef NODES_HPP
#define NODES_HPP

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

#endif