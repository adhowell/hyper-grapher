#include <QGraphicsView>

#pragma once


namespace gui
{
class HyperGraphView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit HyperGraphView(QGraphicsScene* scene, QWidget* parent = nullptr);
    ~HyperGraphView() override = default;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    QPointF mLastPos;
};
}