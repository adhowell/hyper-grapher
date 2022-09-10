#include "hyper_edge.h"

#include <utility>
#include <vector>

#pragma once


namespace core {
/**
 * A hyper-edge is a connection between multiple nodes
 */
class DirectionalHyperEdge : public HyperEdge {
public:
    explicit DirectionalHyperEdge(uint32_t uuid, std::vector<Node> srcNodes, std::vector<Node> dstNodes);
    ~DirectionalHyperEdge() = default;

private:
    size_t mStartDst;
};
}