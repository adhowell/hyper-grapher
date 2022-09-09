#include "items/include/node_item.h"

#include <QtMath>
#include <QPainter>


namespace gui
{
    QRectF NodeItem::boundingRect() const
    {
        return {-mRadius, -mRadius, mRadius*2.0, mRadius*2.0};
    }

    void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(QColor(0, 0, 0));
        painter->setBrush(QColor(255, 255, 255));
        painter->drawEllipse(QRectF(-mRadius, -mRadius, mRadius*2.0, mRadius*2.0));
    }
}