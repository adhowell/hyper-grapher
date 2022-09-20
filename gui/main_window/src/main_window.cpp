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
    connect(mScene, &HyperGraphScene::updateNumSelected, this, &MainWindow::handleUpdateNumSelected);

    mNumSelectedLabel = new QLabel();

    auto layout = new QVBoxLayout;
    layout->addWidget(mScene->getView(), 15);
    layout->addWidget(mNumSelectedLabel);
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

void MainWindow::handleUpdateNumSelected(int numSelected)
{
    mNumSelectedLabel->setText(numSelected > 0 ? QString::number(numSelected) + " Nodes" : "");
}
}