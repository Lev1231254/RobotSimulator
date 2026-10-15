#ifndef NODES_HPP
#define NODES_HPP


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

#endif