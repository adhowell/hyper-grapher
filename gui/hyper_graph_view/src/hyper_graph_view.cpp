#include "hyper_graph_view/include/hyper_graph_view.h"
#include "items/include/edge_item.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsItem>


namespace gui
{
HyperGraphView::HyperGraphView(QGraphicsScene* scene, ProceduralView* viewItem, QWidget* parent) : QGraphicsView(parent), mViewItem(viewItem)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setScene(scene);
    setAlignment(Qt::AlignTop | Qt::AlignLeft);
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
    mViewItem->applyPositionDelta(pos - mLastPos);
    mLastPos = pos;
    update();
}

void HyperGraphView::wheelEvent(QWheelEvent* event)
{
    mViewItem->updateZoom(mapToScene(event->position().x(), event->position().y()), event->angleDelta().y() > 0 ? 1.25 : 0.8);
    event->accept();
}
}
