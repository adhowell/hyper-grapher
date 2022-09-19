#include "main_window/include/main_window.h"
#include "model/include/hyper_graph.h"

#include <QHBoxLayout>


namespace gui
{
MainWindow::MainWindow(QWidget* parent)
{
    core::HyperGraph graph;

    auto last = graph.createHyperEdge({core::Node(0)});
    uint32_t curr = 0;
    for (uint32_t i = 1; i < 10000; i+=2) {
        curr = graph.createHyperEdge({core::Node(i)}, {core::Node(i+1)});
        graph.createMetaEdge(graph.getHyperEdge(last), graph.getHyperEdge(curr));
        last = curr;
    }
    mScene = new HyperGraphScene(graph);

    auto layout = new QHBoxLayout;
    layout->addWidget(mScene->getView());
    setLayout(layout);

    setWindowTitle("Hyper-Grapher");
    setMinimumSize(600, 400);

    mScene->updateRect();
    mScene->startTimer(std::chrono::milliseconds(50));
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    mScene->updateRect();
}
}