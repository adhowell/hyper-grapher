#include "hyper_graph_view/include/hyper_graph_scene.h"
#include "items/include/edge_item.h"

#include <QMap>


namespace gui
{
HyperGraphScene::HyperGraphScene(core::HyperGraph& graph) : QGraphicsScene()
{
    int x = 0;
    int y = 0;
    int maxX = qSqrt(qreal(graph.getHyperEdges().size()));
    for (auto& hyperEdge : graph.getHyperEdges()) {
        mNodes.emplace_back(ProceduralNode{qreal(x), qreal(y), true});
        x += 2;
        if (x > maxX) {
            y += 2;
            x = 0;
        }
    }
    mProceduralView = new ProceduralView(mNodes);
    mView = new HyperGraphView(this, mProceduralView);
    addItem(mProceduralView);
    /*
        auto node = new NodeItem();
        addItem(node);
        node->setPos(i, i);
        node->setZValue(1);
        hyperEdgeItems[hyperEdge.getUuid()] = node;
        i += 50;
        for (auto& c : hyperEdge.getContents()) {
            auto subNode = new NodeItem(node);
            node->addChild(subNode);
            subNode->setPos(i, i-20);
            subNode->setZValue(1);
            addItem(subNode);
        }
    }
    for (const auto& metaEdge : graph.getMetaEdges()) {
        auto edge = new EdgeItem(hyperEdgeItems[metaEdge.getSrcUuid()], hyperEdgeItems[metaEdge.getDstUuid()]);
        hyperEdgeItems[metaEdge.getSrcUuid()]->addEdge(edge);
        addItem(edge);
    }
     */
    //setSceneRect({-1000, -1000, 2000, 2000});
}
}
