#include "items/include/edge_item.h"

#include <QtMath>
#include <QPainter>


namespace gui
{
    QRectF EdgeItem::boundingRect() const
    {
        QPointF p1 = mSrc->pos();
        QPointF p2 = mDst->pos();
        qreal x1 = p1.x();
        qreal y1 = p1.y();
        qreal x2 = p2.x();
        qreal y2 = p2.y();
        return {0, 0, x2-x1, y2-y1};
    }

    void EdgeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        QPointF p1 = mSrc->pos();
        QPointF p2 = mDst->pos();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(QColor(0, 0, 0));
        painter->drawLine(QLineF(0, 0, p2.x()-p1.x(), p2.y()-p1.y()));
    }
}