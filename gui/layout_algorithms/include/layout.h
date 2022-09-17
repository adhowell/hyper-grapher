#include "items/include/procedural_edge.h"

#pragma once


namespace gui
{
class Layout
{
public:
    Layout(std::vector<ProceduralNode*> &nodes, std::vector<ProceduralEdge*> &edges) : mNodes(nodes), mEdges(edges) {};
    virtual ~Layout() = default;

    virtual void calculateNext() = 0;

protected:
    std::vector<ProceduralNode*> &mNodes;
    std::vector<ProceduralEdge*> &mEdges;
};
}