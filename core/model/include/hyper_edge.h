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
    explicit HyperEdge(uint32_t uuid, const std::vector<Entity*>& nodes)
    : Entity(uuid), mContents(nodes)
    {
    }

    virtual ~HyperEdge() = default;

    std::vector<Entity*>& getContents() { return mContents; }

protected:
    std::vector<Entity*> mContents;
};
}