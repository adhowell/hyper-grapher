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
    explicit HyperEdge(uint32_t uuid, const std::vector<Node>& srcNodes, const std::vector<Node>& dstNodes)
    : HyperEdge(uuid, ([&srcNodes, &dstNodes]()
    {
        auto combinedNodes = srcNodes;
        for (auto n : dstNodes) {
            combinedNodes.push_back(n);
        }
        return combinedNodes;
    }()))
    {
        mNumSrc = srcNodes.size();
        mIsDirectional = true;
    }

    explicit HyperEdge(uint32_t uuid, const std::vector<Node>& nodes)
    : Entity(uuid), mContents(nodes)
    {
    }

    virtual ~HyperEdge() = default;

    std::vector<Node>& getContents() { return mContents; }
    __gnu_cxx::__normal_iterator<Node*, std::vector<Node>> getSeparator() { return std::next(mContents.begin(), mNumSrc); }

    bool isDirectional() const { return mIsDirectional; }

protected:
    std::vector<Node> mContents;
    bool mIsDirectional = false;
    size_t mNumSrc = 0;
};
}