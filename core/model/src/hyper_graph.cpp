#include "model/include/hyper_graph.h"

#include <utility>


namespace core
{
    Node* HyperGraph::createNode(int label)
    {
        if (mLabelToNode.find(label) != mLabelToNode.end()) {
            return mLabelToNode[label];
        }
        auto n = new Node(mUuid++);
        mLabelToNode[label] = n;
        return n;
    }

    HyperEdge* HyperGraph::createHyperEdge(std::vector<Entity*> nodes)
    {
        auto edge = new HyperEdge(mUuid++, nodes);
        mHyperEdges.emplace_back(edge);
        return edge;
    }

    HyperEdge* HyperGraph::getHyperEdge(uint32_t uuid)
    {
        for (const auto& hyperEdge : mHyperEdges) {
            if (uuid == hyperEdge->getUuid()) {
                return hyperEdge;
            }
        }
        throw std::exception();
    }
}