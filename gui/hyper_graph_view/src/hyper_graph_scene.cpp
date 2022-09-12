#include "hyper_graph_view/include/hyper_graph_scene.h"
#include "items/include/edge_item.h"

#include <QMap>


namespace gui
{
HyperGraphScene::HyperGraphScene(core::HyperGraph& graph) : QGraphicsScene() {
    int x = 0;
    int y = 0;
    int maxX = qSqrt(qreal(graph.getHyperEdges().size())) * 5;
    std::unordered_map<uint32_t, ProceduralNode*> nodeMap;
    for (auto& hyperEdge: graph.getHyperEdges()) {
        auto node = new ProceduralNode{qreal(x), qreal(y), true};
        nodeMap[hyperEdge.getUuid()] = node;
        mNodes.emplace_back(node);
        x += 5;
        if (x > maxX) {
            y += 5;
            x = 0;
        }
    }
    for (auto& metaEdge : graph.getMetaEdges()) {
        auto edge = new ProceduralEdge{nodeMap.at(metaEdge.getSrcUuid()), nodeMap.at(metaEdge.getDstUuid())};
        mEdges.emplace_back(edge);
    }
    mProceduralView = new ProceduralView(mNodes, mEdges);
    mView = new HyperGraphView(this, mProceduralView);
    addItem(mProceduralView);
}
}
