#include "procedural_node.h"

#include <QtDebug>

#pragma once


namespace gui
{
struct ProceduralEdge
{
    ProceduralNode* src;
    ProceduralNode* dst;
    int weight {0};

    ProceduralEdge(ProceduralNode* s, ProceduralNode* d) : src(s), dst(d) { s->size++; d->size++; };
    ProceduralEdge(ProceduralNode* s, ProceduralNode* d, int w) : src(s), dst(d), weight(w) { s->size++; d->size++; };
};
}