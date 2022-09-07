#include "include/meta_edge.h"


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

private:
    uint32_t mUuid = 0;
    std::vector<MetaEdge> mMetaEdges;
    std::vector<HyperEdge> mHyperEdges;
};
}