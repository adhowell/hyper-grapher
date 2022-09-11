#include "hyper_graph_view/include/hyper_graph_scene.h"
#include "items/include/edge_item.h"

#include <QMap>


namespace gui
{
HyperGraphScene::HyperGraphScene(core::HyperGraph& graph) : QGraphicsScene() {
    int x = 0;
    int y = 0;
    int maxX = qSqrt(qreal(graph.getHyperEdges().size())) * 5;
    for (auto &hyperEdge: graph.getHyperEdges()) {
        mNodes.emplace_back(ProceduralNode{qreal(x), qreal(y), true});
        x += 5;
        if (x > maxX) {
            y += 5;
            x = 0;
        }
    }
    mProceduralView = new ProceduralView(mNodes);
    mView = new HyperGraphView(this, mProceduralView);
    addItem(mProceduralView);
}
}
