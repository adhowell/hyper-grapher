#include "include/hyper_edge.h"


namespace core {
/**
 * A meta-edge connects two hyper-edges.
 */
class MetaEdge : public Entity {
public:
    MetaEdge(uint32_t uuid, const HyperEdge &src, const HyperEdge &dst) : Entity(uuid), mSrc(src), mDst(dst) {}
    ~MetaEdge() = default;

private:
    HyperEdge mSrc;
    HyperEdge mDst;
};
}