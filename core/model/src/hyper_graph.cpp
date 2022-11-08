#include "model/include/hyper_graph.h"

#include <utility>
#include <random>


namespace core
{
Node* HyperGraph::createNode(int label)
{
    if (mLabelToNode.find(label) != mLabelToNode.end()) {
        return mLabelToNode[label];
    }
    auto n = new Node(mUuid++);
    mLabelToNode[label] = n;
    return n;
}

HyperEdge* HyperGraph::createHyperEdge(std::vector<Entity*> nodes)
{
    auto edge = new HyperEdge(mUuid++, nodes);
    mHyperEdges.emplace_back(edge);
    return edge;
}

HyperEdge* HyperGraph::getHyperEdge(uint32_t uuid)
{
    for (const auto& hyperEdge : mHyperEdges) {
        if (uuid == hyperEdge->getUuid()) {
            return hyperEdge;
        }
    }
    throw std::exception();
}

core::HyperGraph HyperGraph::createRandomGraph()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> numNodesDist(1.0, 250.0);
    std::uniform_real_distribution<double> edgeSizeDist(1.0, 5.0);

    core::HyperGraph graph;
    for (uint32_t i = 0; i < 100; i++) {
        graph.createHyperEdge([&mt, &edgeSizeDist, &numNodesDist, &graph]()
                              {
                                  std::vector<core::Entity*> nodes;
                                  for (int j = 0; j < edgeSizeDist(mt); j++) {
                                      nodes.push_back(graph.createNode((int)numNodesDist(mt)));
                                  }
                                  return nodes;
                              }());
    }
    return graph;
}
}