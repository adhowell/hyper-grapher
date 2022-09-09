#include "node.h"
#include "entity.h"

#include <utility>
#include <vector>

#pragma once


namespace core {
/**
 * A hyper-edge is a connection between multiple nodes
 */
class HyperEdge : public Entity {
public:
    explicit HyperEdge(uint32_t uuid, std::vector<Node> nodes) : Entity(uuid), mContents(std::move(nodes)) {}
    ~HyperEdge() = default;

private:
    std::vector <Node> mContents;
};
}