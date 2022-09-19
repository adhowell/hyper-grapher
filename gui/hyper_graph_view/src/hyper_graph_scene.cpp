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

    // max this entire part is a placeholder so you better not start judging me
    std::unordered_map<uint32_t, ProceduralNode*> hyperEdgeToPN;
    std::unordered_map<uint32_t, ProceduralNode*> nodeToPN;
    for (auto& hyperEdge: graph.getHyperEdges()) {
        auto node = new ProceduralNode{qreal(x), qreal(y), true};
        hyperEdgeToPN[hyperEdge.getUuid()] = node;
        mNodes.emplace_back(node);
        if (hyperEdge.isDirectional()) {
            auto joinNode = new ProceduralNode{qreal(x), qreal(y), false, node};
            mNodes.emplace_back(joinNode);

            // Src nodes -> join node
            std::for_each(hyperEdge.getContents().begin(), hyperEdge.getSeparator(),
                          [this, x, y, node, joinNode, &nodeToPN](auto n)
                          {
                              auto pn = new ProceduralNode{qreal(x), qreal(y), false, node};
                              nodeToPN[n.getUuid()] = pn;
                              mNodes.emplace_back(pn);
                              mEdges.emplace_back(new ProceduralEdge{pn, joinNode});
                          });

            // Join node -> dst nodes
            std::for_each(hyperEdge.getSeparator(), hyperEdge.getContents().end(),
                          [this, x, y, node, joinNode, &nodeToPN](auto n)
                          {
                              auto pn = new ProceduralNode{qreal(x), qreal(y), false, node};
                              nodeToPN[n.getUuid()] = pn;
                              mNodes.emplace_back(pn);
                              mEdges.emplace_back(new ProceduralEdge{joinNode, pn});
                          });
        }
        x += 5;
        if (x > maxX) {
            y += 5;
            x = 0;
        }
    }
    for (auto& metaEdge : graph.getMetaEdges()) {
        auto edge = new ProceduralEdge{hyperEdgeToPN.at(metaEdge.getSrcUuid()), hyperEdgeToPN.at(metaEdge.getDstUuid())};
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
