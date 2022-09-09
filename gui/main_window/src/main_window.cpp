#include "main_window/include/main_window.h"
#include "model/include/hyper_graph.h"


#include <QHBoxLayout>


namespace gui
{
MainWindow::MainWindow(QWidget* parent) {
    //setStyleSheet("color: #00ff00; background-color: black;");

    core::HyperGraph graph;
    auto e1 = graph.createHyperEdge({core::Node(0), core::Node(1), core::Node(2)});
    auto e2 = graph.createHyperEdge({core::Node(3), core::Node(4)});
    auto e3 = graph.createHyperEdge({core::Node(5)});
    graph.createMetaEdge(graph.getHyperEdge(e1), graph.getHyperEdge(e2));
    graph.createMetaEdge(graph.getHyperEdge(e2), graph.getHyperEdge(e3));
    mScene = new HyperGraphScene(graph);

    auto layout = new QHBoxLayout;
    layout->addWidget(mScene->getView());
    setLayout(layout);

    setWindowTitle("Hyper Grapher");
    setMinimumSize(500, 500);
}
}