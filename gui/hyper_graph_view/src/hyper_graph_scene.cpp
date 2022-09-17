#include "hyper_graph_view/include/hyper_graph_scene.h"
#include "items/include/edge_item.h"
#include "layout_algorithms/include/edge_attraction.h"

#include <QMap>
#include <thread>


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

    connect(mView, &HyperGraphView::toggleAlgorithmActive,
            this, &HyperGraphScene::handleToggleAlgorithmActive);

    std::thread layoutThread (layoutThreadWorker, mNodes, mEdges, std::ref(mLayoutAlgorithmActive));
    layoutThread.detach();
}

void HyperGraphScene::layoutThreadWorker(std::vector<ProceduralNode*> nodes, std::vector<ProceduralEdge*> edges, std::atomic<bool>& isActive)
{
    auto layoutAlgorithm = EdgeAttraction(nodes, edges);
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds (50));
        if (isActive) {
            layoutAlgorithm.calculateNext();
        }
    }
}

void HyperGraphScene::timerEvent(QTimerEvent *event)
{
    if (mLayoutAlgorithmActive) {
        mView->updateFrame();
    }
}
}
