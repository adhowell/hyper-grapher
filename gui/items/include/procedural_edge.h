#include "procedural_node.h"

#include <QtDebug>


namespace gui
{
struct ProceduralEdge
{
    ProceduralNode* src;
    ProceduralNode* dst;
};
}