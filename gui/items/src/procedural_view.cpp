#include "items/include/procedural_view.h"

#include <algorithm>
#include <QPainter>


namespace gui
{
ProceduralView::ProceduralView(std::vector<ProceduralNode> &nodes) : QGraphicsItem(), mNodes(nodes)
{
    bool first = true;
    for (auto n : nodes) {
        if (first || n.x < mX1) mX1 = n.x;
        if (first || n.x > mX2) mX2 = n.x;
        if (first || n.y < mY1) mY1 = n.y;
        if (first || n.y > mY2) mY2 = n.y;
        first = false;
    }
    // Margins
    mX1 -= 25;
    mX2 += 25;
    mY1 -= 25;
    mY2 += 25;
    slowUpdate(); // Because we need to init the iter
}

void ProceduralView::slowUpdate()
{
    mIt = std::partition(mNodes.begin(), mNodes.end(),
                         [this](auto n)
                         {
        bool b = (mX2 > n.x && n.x > mX1 && mY2 > n.y && n.y > mY1);
                             return b;
                         });
    update();
}

void ProceduralView::lessSlowUpdate()
{
    mIt = std::partition(mNodes.begin(), mIt,
                         [this](auto n)
                         {
                             bool b = (mX2 > n.x && n.x > mX1 && mY2 > n.y && n.y > mY1);
                             return b;
                         });
    update();
}

void ProceduralView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QColor(0, 0, 0));
    std::for_each(mNodes.begin(), mIt,
                  [painter, this](auto n)
                  {
                    painter->drawEllipse(mRect.width()*(n.x-mX1)/(mX2-mX1)-5, mRect.height()*(n.y-mY1)/(mY2-mY1)-5, 10, 10);
                  });
}

void ProceduralView::updateZoom(QPointF newCentre, qreal zoomFactor)
{
    qreal x = (mX2-mX1)*newCentre.x()/mRect.width() + mX1;
    qreal y = (mY2-mY1)*newCentre.y()/mRect.height() + mY1;
    qreal currWidth = mX2 - mX1;
    qreal currHeight = mY2 - mY1;
    mX1 = x - currWidth * 0.5 / zoomFactor;
    mX2 = x + currWidth * 0.5 / zoomFactor;
    mY1 = y - currHeight * 0.5 / zoomFactor;
    mY2 = y + currHeight * 0.5 / zoomFactor;
    if (zoomFactor > 1) {
        lessSlowUpdate();
    } else {
        slowUpdate();
    }
}

void ProceduralView::applyPositionDelta(QPointF delta)
{
    qreal deltaX = (mX2-mX1)*delta.x()/mRect.width();
    qreal deltaY = (mY2-mY1)*delta.y()/mRect.height();
    mX1 -= deltaX;
    mX2 -= deltaX;
    mY1 -= deltaY;
    mY2 -= deltaY;
    slowUpdate();
    update();
}

void ProceduralView::setRect(QRectF rect)
{
    mRect = rect;
    slowUpdate();
    update();
}
}