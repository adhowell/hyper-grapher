#include "main_window/include/main_window.h"
#include "model/include/hyper_graph.h"

#include <QHBoxLayout>


namespace gui
{
MainWindow::MainWindow(QWidget* parent)
{
    core::HyperGraph graph;

    for (uint32_t i = 0; i < 10000; i++) {
        graph.createHyperEdge({graph.createNode(i), graph.createNode(i+1), graph.createNode(i+2)});
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