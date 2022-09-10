#include "items/include/edge_item.h"

#include <QtMath>
#include <QPainter>


namespace gui
{
    QRectF EdgeItem::boundingRect() const
    {
        return {-1000, -1000, 2000, 2000};
    }

    void EdgeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(QColor(0, 0, 0));

        if (mSrc->getNodePositions().empty() || mDst->getNodePositions().empty()) {
            return;
        }
        QPointF startPos = mSrc->getNodePositions()[0];
        setPos(startPos);
        for (auto src : mSrc->getNodePositions()) {
            for (auto dst : mDst->getNodePositions()) {
                painter->drawLine(QLineF(src.x()-startPos.x(), src.y()-startPos.y(), dst.x()-startPos.x(), dst.y()-startPos.y()));
            }
        }
    }
}