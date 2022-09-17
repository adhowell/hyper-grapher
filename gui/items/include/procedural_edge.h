#include "procedural_node.h"

#include <QtDebug>

#pragma once


namespace gui
{
struct ProceduralEdge
{
    ProceduralNode* src;
    ProceduralNode* dst;
};
}