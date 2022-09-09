#include "hyper_graph_view/include/hyper_graph_scene.h"
#include "items/include/edge_item.h"

#include <QMap>


namespace gui
{
HyperGraphScene::HyperGraphScene(core::HyperGraph& graph) : QGraphicsScene()
{
    mView = new HyperGraphView(this);
    int i = 0;
    QMap<uint32_t, NodeItem*> hyperEdgeItems {};
    for (const auto& hyperEdge : graph.getHyperEdges()) {
        auto node = new NodeItem();
        addItem(node);
        node->setPos(i, i);
        node->setZValue(1);
        hyperEdgeItems[hyperEdge.getUuid()] = node;
        i += 50;
    }
    for (const auto& metaEdge : graph.getMetaEdges()) {
        auto edge = new EdgeItem(hyperEdgeItems[metaEdge.getSrcUuid()], hyperEdgeItems[metaEdge.getDstUuid()]);
        addItem(edge);
        edge->setPos(hyperEdgeItems[metaEdge.getSrcUuid()]->pos());
    }
    //setSceneRect({-1000, -1000, 2000, 2000});
}
}
