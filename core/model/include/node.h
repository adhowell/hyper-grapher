#include <cstdint>

#pragma once


namespace core {
/**
 * Nodes are the least significant element in the graph hierarchy.
 */
class Node {
public:
    explicit Node(uint32_t uuid) : mUuid(uuid) {}
    ~Node() = default;

    uint32_t getUuid() const { return mUuid; }

private:
    uint32_t mUuid;
};
}