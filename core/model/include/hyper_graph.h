#include "hyper_edge.h"

#include <unordered_map>

#pragma once


namespace core {
/**
 * A hyper-graph associates meta-edges, hyper-edges and nodes.
 */
class HyperGraph {
public:
    HyperGraph() = default;
    ~HyperGraph() = default;

    Node* createNode(int label);
    HyperEdge* createHyperEdge(std::vector<Entity*> nodes);

    std::vector<HyperEdge*>& getHyperEdges() { return mHyperEdges; }

    HyperEdge* getHyperEdge(uint32_t uuid);

private:
    uint32_t mUuid = 0;
    std::unordered_map<int, Node*> mLabelToNode; // Placeholder
    std::vector<HyperEdge*> mHyperEdges;
};
}