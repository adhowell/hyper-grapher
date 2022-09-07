#include <utility>

#include "include/hyper_graph.h"

namespace core
{
    uint32_t HyperGraph::createHyperEdge(std::vector<Node> nodes)
    {
        auto edge = HyperEdge(mUuid++, std::move(nodes));
        mHyperEdges.emplace_back(edge);
        return edge.getUuid();
    }

    uint32_t HyperGraph::createMetaEdge(const HyperEdge& src, const HyperEdge& dst)
    {
        auto edge = MetaEdge(mUuid++, src, dst);
        mMetaEdges.emplace_back(edge);
        return edge.getUuid();
    }
}