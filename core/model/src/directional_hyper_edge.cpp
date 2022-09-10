#include "model/include/directional_hyper_edge.h"

#include <algorithm>


namespace core
{
    DirectionalHyperEdge::DirectionalHyperEdge(uint32_t uuid, std::vector<Node> srcNodes, std::vector<Node> dstNodes)
    : HyperEdge(uuid, [&srcNodes, &dstNodes]
    {
        for (auto n : dstNodes) {
            srcNodes.push_back(n);
        }
        return srcNodes;
    }())
    {
        mStartDst = srcNodes.size();
    }
}