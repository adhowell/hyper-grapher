#include "layout.h"


namespace gui
{
class EdgeAttraction : public Layout
{
public:
    EdgeAttraction(std::vector<ProceduralNode*> &nodes, std::vector<ProceduralEdge*> &edges)
    : Layout(nodes, edges) {};
    ~EdgeAttraction() override = default;

    void calculateNext() override;

private:
    double mAttraction {0.001};
    double mRestingDistance {1.0};
};
}
