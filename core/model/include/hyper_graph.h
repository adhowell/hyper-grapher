#include "meta_edge.h"

#pragma once


namespace core {
/**
 * A hyper-graph associates meta-edges, hyper-edges and nodes.
 */
class HyperGraph {
public:
    HyperGraph() = default;
    ~HyperGraph() = default;

    uint32_t createHyperEdge(std::vector<Node> nodes);
    uint32_t createHyperEdge(std::vector<Node> srcNodes, std::vector<Node> dstNodes);
    uint32_t createMetaEdge(HyperEdge src, HyperEdge dst);

    std::vector<HyperEdge>& getHyperEdges() { return mHyperEdges; }
    std::vector<MetaEdge>& getMetaEdges() { return mMetaEdges; }

    HyperEdge getHyperEdge(uint32_t uuid);

private:
    uint32_t mUuid = 0;
    std::vector<MetaEdge> mMetaEdges;
    std::vector<HyperEdge> mHyperEdges;
};
}