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
        case Qt::Key_A: mViewItem->selectAllVisible(); break;
        case Qt::Key_D: mViewItem->toggleDrawDetails(); break;
        case Qt::Key_E: mViewItem->toggleDrawEdges(); break;
        case Qt::Key_Space: mState = ClickDragState::Move; break;
    }
}

void HyperGraphView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        mState = ClickDragState::Select;
    }
}

void HyperGraphView::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);
    event->accept();
    mLastPos = mapToScene(event->pos());
    if (mState == ClickDragState::Move) {
        return;
    }
    if (event->button() & Qt::RightButton) {
        mState = ClickDragState::Pan;
    } else if (event->button() & Qt::LeftButton) {
        mViewItem->deselectAll();
        mViewItem->updateSelectionBoxStart(mapToScene(event->pos()));
        mState = ClickDragState::Select;
    }
}

void HyperGraphView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event); // Otherwise items don't handle mouse-move events
    event->accept();
    switch (mState) {
        case ClickDragState::Pan:
            mViewItem->applyPositionDelta(mapToScene(event->pos()) - mLastPos);
            mLastPos = mapToScene(event->pos());
            break;
        case ClickDragState::Select:
            mViewItem->drawBox(true);
            mViewItem->updateSelectionBoxEnd(mapToScene(event->pos()));
            break;
        case ClickDragState::Move:
            mViewItem->applyFocusPositionDelta(mapToScene(event->pos()) - mLastPos);
            mLastPos = mapToScene(event->pos());
            break;
    }
}

void HyperGraphView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    event->accept();
    if (event->buttons() & Qt::LeftButton) {
        mState = ClickDragState::Select;
    } else {
        mViewItem->drawBox(false);
    }
    mViewItem->update();
}

void HyperGraphView::wheelEvent(QWheelEvent* event)
{
    mViewItem->updateZoom(mapToScene(event->position().x(), event->position().y()), event->angleDelta().y() > 0 ? 1.25 : 0.8);
    event->accept();
}
}
