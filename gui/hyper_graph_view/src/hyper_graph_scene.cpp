#include "hyper_graph_view/include/hyper_graph_scene.h"
#include "items/include/edge_item.h"
#include "layout_algorithms/include/edge_attraction.h"

#include <QMap>
#include <thread>


namespace gui
{
using PNT = ProceduralNode::Type;

HyperGraphScene::HyperGraphScene(core::HyperGraph& graph) : QGraphicsScene() {
    int x = 0;
    int y = 0;
    int maxX = qSqrt(qreal(graph.getHyperEdges().size())) * 5;

    // max this entire part is a placeholder so you better not start judging me
    std::unordered_map<uint32_t, ProceduralNode*> entityToPN;
    for (auto& hyperEdge: graph.getHyperEdges()) {
        if (entityToPN.find(hyperEdge->getUuid()) == entityToPN.end()) {
            auto hyperEdgeNode = new ProceduralNode{qreal(x), qreal(y), true, PNT::Edge};
            entityToPN[hyperEdge->getUuid()] = hyperEdgeNode;
            mNodes.emplace_back(hyperEdgeNode);
        }
        auto node = entityToPN[hyperEdge->getUuid()];
        if (hyperEdge->getContents().size() > 2) {
            auto joinNode = new ProceduralNode{qreal(x), qreal(y), false, PNT::Join};
            mNodes.emplace_back(joinNode);

            std::for_each(hyperEdge->getContents().begin(), hyperEdge->getContents().end(),
                          [this, x, y, node, joinNode, &entityToPN](auto n)
                          {
                              if (entityToPN.find(n->getUuid()) == entityToPN.end()) {
                                  auto pn = new ProceduralNode{qreal(x), qreal(y), false, PNT::Node};
                                  entityToPN[n->getUuid()] = pn;
                                  mNodes.emplace_back(pn);
                              }
                              auto foundPN = entityToPN[n->getUuid()];
                              joinNode->addParent(node);
                              foundPN->addParent(node);
                              mEdges.emplace_back(new ProceduralEdge{foundPN, joinNode});
                          });
        } else {
            auto n1 = hyperEdge->getContents()[0];
            auto n2 = hyperEdge->getContents()[1];
            if (entityToPN.find(n1->getUuid()) == entityToPN.end()) {
                auto pn = new ProceduralNode{qreal(x), qreal(y), false, PNT::Node};
                entityToPN[n1->getUuid()] = pn;
                mNodes.emplace_back(pn);
            }
            if (entityToPN.find(n2->getUuid()) == entityToPN.end()) {
                auto pn = new ProceduralNode{qreal(x), qreal(y), false, PNT::Node};
                entityToPN[n2->getUuid()] = pn;
                mNodes.emplace_back(pn);
            }
            auto pn1 = entityToPN[n1->getUuid()];
            auto pn2 = entityToPN[n2->getUuid()];
            pn1->addParent(node);
            pn2->addParent(node);
            mEdges.emplace_back(new ProceduralEdge{pn1, pn2});
        }
        x += 5;
        if (x > maxX) {
            y += 5;
            x = 0;
        }
    }

    // Construct additional edges based on nodes sharing parents
    for (auto [_, n] : entityToPN) {
        for (auto iterI = n->parents.begin(); iterI != n->parents.end(); iterI++) {
            for (auto iterJ = std::next(iterI, 1); iterJ != n->parents.end(); iterJ++) {
                mEdges.emplace_back(new ProceduralEdge{*iterI, *iterJ});
            }
        }
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
