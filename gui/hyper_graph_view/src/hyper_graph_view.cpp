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
    QGraphicsView::mousePressEvent(event);
    event->accept();
    if (!(event->button() & Qt::RightButton)) {
        return;
    }
    mLastPos = mapToScene(event->pos());
}

void HyperGraphView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event); // Otherwise items don't handle mouse-move events
    event->accept();
    if (!(event->buttons() & Qt::RightButton)) {
        return;
    }
    auto pos = mapToScene(event->pos());
    itemsApplyDelta(pos - mLastPos);
    mLastPos = pos;
    update();
}

void HyperGraphView::wheelEvent(QWheelEvent* event)
{
    qreal delta = event->angleDelta().y() > 0 ? 1.25 : 0.8;
    scale(delta, delta);
    event->accept();
}

void HyperGraphView::itemsApplyDelta(QPointF delta)
{
    for (auto item : items()) {
        item->setPos(item->pos().x() + delta.x(), item->pos().y() + delta.y());
    }
}
}
