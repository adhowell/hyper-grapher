#include "layout.h"


namespace gui
{
class NodeRepulsionEdgeAttraction : public Layout
{
public:
    NodeRepulsionEdgeAttraction(std::vector<ProceduralNode*> &nodes, std::vector<ProceduralEdge*> &edges)
    : Layout(nodes, edges) {};
    ~NodeRepulsionEdgeAttraction() override = default;

    void calculateNext() override;

private:
    double mAttraction {0.001};
    double mRestingDistance {1.0};
};
}
