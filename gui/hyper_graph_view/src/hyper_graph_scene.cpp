#include "hyper_graph_view/include/hyper_graph_scene.h"
#include "items/include/edge_item.h"
#include "layout_algorithms/include/edge_attraction.h"
#include "layout_algorithms/include/node_repulsion_edge_attraction.h"

#include <QMap>
#include <thread>


namespace gui
{
using PNT = ProceduralNode::Type;

HyperGraphScene::HyperGraphScene(core::HyperGraph& graph) : QGraphicsScene() {
    int x = 0;
    int y = 0;
    int maxX = qSqrt(qreal(graph.getHyperEdges().size())) * 5;

    auto keyGen = [](ProceduralNode* n1, ProceduralNode* n2)
    {
        uint32_t small = qMin(n1->uuid, n2->uuid);
        uint64_t big = static_cast<uint64_t>(qMax(n1->uuid, n2->uuid));
        return small | big << 32;
    };

    // max this entire part is a placeholder so you better not start judging me
    std::unordered_map<uint32_t, ProceduralNode*> entityToProcNode;
    std::unordered_map<uint64_t, ProceduralEdge*> procNodesToEdge;
    for (auto& hyperEdge: graph.getHyperEdges()) {
        if (entityToProcNode.find(hyperEdge->getUuid()) == entityToProcNode.end()) {
            auto hyperEdgeNode = new ProceduralNode{hyperEdge->getUuid(), qreal(x), qreal(y), true, PNT::Edge};
            entityToProcNode[hyperEdge->getUuid()] = hyperEdgeNode;
            mNodes.emplace_back(hyperEdgeNode);
        }
        auto node = entityToProcNode[hyperEdge->getUuid()];
        if (hyperEdge->getContents().size() > 2) {
            auto joinNode = new ProceduralNode{hyperEdge->getUuid(), qreal(x), qreal(y), false, PNT::Join};
            mNodes.emplace_back(joinNode);

            std::for_each(hyperEdge->getContents().begin(), hyperEdge->getContents().end(),
                          [this, x, y, node, joinNode, &entityToProcNode](auto n)
                          {
                              if (entityToProcNode.find(n->getUuid()) == entityToProcNode.end()) {
                                  auto pn = new ProceduralNode{n->getUuid(), qreal(x), qreal(y), false, PNT::Node};
                                  entityToProcNode[n->getUuid()] = pn;
                                  mNodes.emplace_back(pn);
                              }
                              auto foundPN = entityToProcNode[n->getUuid()];
                              joinNode->addParent(node);
                              foundPN->addParent(node);
                              mEdges.emplace_back(new ProceduralEdge(foundPN, joinNode, 1));
                          });
        } else {
            auto n1 = hyperEdge->getContents()[0];
            auto n2 = hyperEdge->getContents()[1];
            if (entityToProcNode.find(n1->getUuid()) == entityToProcNode.end()) {
                auto pn = new ProceduralNode{n1->getUuid(), qreal(x), qreal(y), false, PNT::Node};
                entityToProcNode[n1->getUuid()] = pn;
                mNodes.emplace_back(pn);
            }
            if (entityToProcNode.find(n2->getUuid()) == entityToProcNode.end()) {
                auto pn = new ProceduralNode{n2->getUuid(), qreal(x), qreal(y), false, PNT::Node};
                entityToProcNode[n2->getUuid()] = pn;
                mNodes.emplace_back(pn);
            }
            auto pn1 = entityToProcNode[n1->getUuid()];
            auto pn2 = entityToProcNode[n2->getUuid()];
            pn1->addParent(node);
            pn2->addParent(node);

            auto nodeKey = keyGen(pn1, pn2);
            if (procNodesToEdge.find(nodeKey) == procNodesToEdge.end()) {
                auto edge = new ProceduralEdge(pn1, pn2);
                mEdges.emplace_back(edge);
                procNodesToEdge[nodeKey] = edge;
            }
            procNodesToEdge[nodeKey]->weight++;
        }
        x += 5;
        if (x > maxX) {
            y += 5;
            x = 0;
        }
    }

    // Construct additional edges based on nodes sharing parents
    for (auto [_, n] : entityToProcNode) {
        for (auto iterI = n->parents.begin(); iterI != n->parents.end(); iterI++) {
            for (auto iterJ = std::next(iterI, 1); iterJ != n->parents.end(); iterJ++) {
                auto nodeKey = keyGen(*iterI, *iterJ);
                if (procNodesToEdge.find(nodeKey) == procNodesToEdge.end()) {
                    auto edge = new ProceduralEdge(*iterI, *iterJ);
                    mEdges.emplace_back(edge);
                    procNodesToEdge[nodeKey] = edge;
                }
                procNodesToEdge[nodeKey]->weight++;
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
    //auto layoutAlgorithm = NodeRepulsionEdgeAttraction(nodes, edges);
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
    Q_EMIT updateNumSelected(mView->getNumSelected());
}
}
