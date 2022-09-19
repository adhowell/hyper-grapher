#include "entity.h"

#include <cstdint>

#pragma once


namespace core {
/**
 * Nodes are the least significant element in the graph hierarchy.
 */
class Node : public Entity {
public:
    explicit Node(uint32_t uuid) : Entity(uuid) {}
    ~Node() = default;
};
}