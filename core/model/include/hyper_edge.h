#include "node.h"
#include "entity.h"

#include <utility>
#include <vector>

#pragma once


namespace core {
/**
 * A hyper-edge is a connection between multiple nodes
 */
class HyperEdge : public Entity {
public:
    explicit HyperEdge(uint32_t uuid, std::vector<Node> nodes) : Entity(uuid), mContents(std::move(nodes)), mStartDst(nodes.size()) {}
    HyperEdge(uint32_t uuid, std::vector<Node> srcNodes, std::vector<Node> dstNodes)
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
    ~HyperEdge() = default;

    std::vector<Node>& getContents() { return mContents; }

private:
    std::vector<Node> mContents;
    size_t mStartDst;
};
}