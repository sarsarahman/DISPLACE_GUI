#ifndef IDTYPES_H
#define IDTYPES_H

#include <iostream>
#include <cstdint>

namespace types {

template <typename C, typename Type>
class Id {
private:
    C id;
public:
    explicit Id(C x) : id(x) {}

    friend std::ostream &operator << (std::ostream &ss, const Id &c) {
        ss << c.id;
        return ss;
    }

    C toIndex() const { return id; }

    friend bool operator == (Id<C,Type> id1, Id<C,Type> id2) {
        return id1.id == id2.id;
    }

    friend bool operator < (Id<C,Type> id1, Id<C,Type> id2) {
        return id1.id < id2.id;
    }

    friend bool operator != (Id<C,Type> id1, Id<C,Type> id2) {
        return id1.id != id2.id;
    }

};


struct NodeTag {};

class NodeId : public Id<uint16_t, NodeTag> {
public:
    NodeId() : Id((uint16_t)-1) {}
    explicit NodeId(uint16_t v) : Id(v) {}
};

namespace special {
    static const NodeId InvalidNodeId;
}

}

#endif // IDTYPES_H
