#include <QGraphicsView>
#include "items/include/procedural_view.h"

#pragma once


namespace gui
{
class HyperGraphView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit HyperGraphView(QGraphicsScene* scene, ProceduralView* viewItem, QWidget* parent = nullptr);
    ~HyperGraphView() override = default;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;

    void updateRect() { mViewItem->setRect(rect()); }

private:
    enum class ClickDragState
    {
        Select,
        Pan,
        Move
    };

    ClickDragState mState;
    QPointF mLastPos;
    ProceduralView* mViewItem;
};
}