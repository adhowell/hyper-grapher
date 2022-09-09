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
    uint32_t createMetaEdge(const HyperEdge& src, const HyperEdge& dst);

    const std::vector<HyperEdge>& getHyperEdges() const { return mHyperEdges; }
    const std::vector<MetaEdge>& getMetaEdges() const { return mMetaEdges; }

    HyperEdge getHyperEdge(uint32_t uuid);

private:
    uint32_t mUuid = 0;
    std::vector<MetaEdge> mMetaEdges;
    std::vector<HyperEdge> mHyperEdges;
};
}