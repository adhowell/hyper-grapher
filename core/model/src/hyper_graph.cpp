#include "model/include/hyper_graph.h"

#include <utility>


namespace core
{
    uint32_t HyperGraph::createHyperEdge(std::vector<Node> nodes)
    {
        auto edge = HyperEdge(mUuid++, std::move(nodes));
        mHyperEdges.emplace_back(edge);
        return edge.getUuid();
    }

    uint32_t HyperGraph::createHyperEdge(std::vector<Node> srcNodes, std::vector<Node> dstNodes)
    {
        auto edge = HyperEdge(mUuid++, std::move(srcNodes), std::move(dstNodes));
        mHyperEdges.emplace_back(edge);
        return edge.getUuid();
    }

    uint32_t HyperGraph::createMetaEdge(HyperEdge src, HyperEdge dst)
    {
        auto edge = MetaEdge(mUuid++, src, dst);
        mMetaEdges.emplace_back(edge);
        return edge.getUuid();
    }

    HyperEdge HyperGraph::getHyperEdge(uint32_t uuid)
    {
        for (const auto& hyperEdge : mHyperEdges) {
            if (uuid == hyperEdge.getUuid()) {
                return hyperEdge;
            }
        }
        throw std::exception();
    }
}