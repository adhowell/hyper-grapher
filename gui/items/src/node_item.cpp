#include "items/include/node_item.h"

#include <QtMath>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>


namespace gui
{
    NodeItem::NodeItem(NodeItem* parent)
    : QGraphicsItem(), mParent(parent), mPen(QColor(0, 0, 0)), mBrush(QColor(255, 255, 255))
    {
        if (parent) {
            hide();
        }
        setAcceptHoverEvents(true);
    }

    QRectF NodeItem::boundingRect() const
    {
        return {-mRadius, -mRadius, mRadius*2.0, mRadius*2.0};
    }

    void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(mPen);
        painter->setBrush(mBrush);
        painter->drawEllipse(QRectF(-mRadius, -mRadius, mRadius*2.0, mRadius*2.0));
        for (auto e : mEdges) {
            e->update();
        }
    }

    void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        event->accept();
    }

    void NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
    {
        if (mChildren.empty()) {
            return;
        }
        for (auto child : mChildren) {
            child->setVisible(true);
        }
        hide();
        update();
        event->accept();
    }

    void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        setPos(mapToScene(event->pos()));
    }

    void NodeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
    {
        mPen.setWidthF(2.0);
        update();
    }

    void NodeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
    {
        mPen.setWidthF(1.0);
        update();
    }

    void NodeItem::addChild(NodeItem* node)
    {
        mChildren.emplace_back(node);
    }

    void NodeItem::addEdge(QGraphicsItem* edge)
    {
        mEdges.emplace_back(edge);
    }

    std::vector<QPointF> NodeItem::getNodePositions()
    {
        //if (!isVisible()) {
        //    return {};
        //}
        if (mChildren.empty()) {
            return {pos()};
        }
        std::vector<QPointF> positions;
        for (auto n : mChildren) {
            if (!n->isVisible()) {
                continue;
            }
            for (auto p : n->getNodePositions()) {
                positions.emplace_back(p);
            }
        }
        if (positions.empty() && isVisible()) {
            return {pos()};
        }
        return positions;
    }
}