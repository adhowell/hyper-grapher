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

void HyperGraphView::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
    switch (event->key()) {
        case Qt::Key_D: mViewItem->toggleDrawDetails(); break;
        case Qt::Key_E: mViewItem->toggleDrawEdges(); break;
    }
}

void HyperGraphView::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);
    event->accept();
    if (event->button() & Qt::RightButton) {
        mLastPos = mapToScene(event->pos());
    } else if (event->button() & Qt::LeftButton) {
        mViewItem->deselectAll();
        mViewItem->updateSelectionBoxStart(mapToScene(event->pos()));
    }
}

void HyperGraphView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event); // Otherwise items don't handle mouse-move events
    event->accept();
    if (event->buttons() & Qt::RightButton) {
        auto pos = mapToScene(event->pos());
        mViewItem->applyPositionDelta(pos - mLastPos);
        mLastPos = pos;
        update();
    } else if (event->buttons() & Qt::LeftButton) {
        mViewItem->drawBox(true);
        mViewItem->updateSelectionBoxEnd(mapToScene(event->pos()));
    }
}

void HyperGraphView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    event->accept();
    mViewItem->drawBox(false);
    mViewItem->update();
}

void HyperGraphView::wheelEvent(QWheelEvent* event)
{
    mViewItem->updateZoom(mapToScene(event->position().x(), event->position().y()), event->angleDelta().y() > 0 ? 1.25 : 0.8);
    event->accept();
}
}
