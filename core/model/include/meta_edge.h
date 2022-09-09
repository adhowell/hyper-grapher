#include "hyper_edge.h"

#pragma once


namespace core {
/**
 * A meta-edge connects two hyper-edges.
 */
class MetaEdge : public Entity {
public:
    MetaEdge(uint32_t uuid, const HyperEdge &src, const HyperEdge &dst) : Entity(uuid), mSrc(src), mDst(dst) {}
    ~MetaEdge() = default;

    uint32_t getSrcUuid() const { return mSrc.getUuid(); }
    uint32_t getDstUuid() const { return mDst.getUuid(); }

private:
    HyperEdge mSrc;
    HyperEdge mDst;
};
}