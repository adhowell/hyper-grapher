#include "hyper_graph_view/include/hyper_graph_view.h"
#include "items/include/edge_item.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsItem>


namespace gui
{
HyperGraphView::HyperGraphView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(scene);
}

void HyperGraphView::mousePressEvent(QMouseEvent* event)
{
    mLastPos = mapToScene(event->pos());
    event->accept();
}

void HyperGraphView::mouseMoveEvent(QMouseEvent* event)
{
    auto pos = mapToScene(event->pos());
    for (auto item : items()) {
        item->setPos(item->pos().x() + pos.x() - mLastPos.x(), item->pos().y() + pos.y() - mLastPos.y());
    }
    mLastPos = pos;
    update();
    event->accept();
}

void HyperGraphView::wheelEvent(QWheelEvent* event)
{
    qreal delta = event->angleDelta().y() > 0 ? 1.25 : 0.8;
    scale(delta, delta);
    event->accept();
}
}
