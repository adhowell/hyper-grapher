#include "items/include/procedural_view.h"

#include <algorithm>
#include <QPainter>


namespace gui
{
ProceduralView::ProceduralView(std::vector<ProceduralNode*> &nodes, std::vector<ProceduralEdge*> &edges) : QGraphicsItem(), mNodes(nodes), mEdges(edges)
{
    bool first = true;
    for (auto n : nodes) {
        if (first || n->x < mX1) mX1 = n->x;
        if (first || n->x > mX2) mX2 = n->x;
        if (first || n->y < mY1) mY1 = n->y;
        if (first || n->y > mY2) mY2 = n->y;
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
                             return n->visible && mX2 > n->x && n->x > mX1 && mY2 > n->y && n->y > mY1;
                         });
    update();
}

void ProceduralView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    qreal wF = mRect.width()/(mX2-mX1);
    qreal hF = mRect.height()/(mY2-mY1);
    painter->setRenderHint(QPainter::Antialiasing);
    auto defaultPen = QPen();
    auto focusPen = QPen();
    focusPen.setWidthF(3);
    if (mDrawDetails) {
        std::for_each(mNodes.begin(), mIt,
                      [painter, wF, hF, focusPen, defaultPen, this](auto n) {
                          painter->setPen(n->focus ? focusPen : defaultPen);
                          painter->drawRect(wF*(n->x-mX1) - 5, hF * (n->y - mY1) - 5, 10, 10);
                      });
    } else {
        std::for_each(mNodes.begin(), mIt,
                      [painter, wF, hF, focusPen, defaultPen, this](auto n) {
                          painter->setPen(n->focus ? focusPen : defaultPen);
                          painter->drawLine(wF*(n->x-mX1), hF*(n->y-mY1), wF*(n->x-mX1), hF*(n->y-mY1));
                      });
    }
    painter->setPen(QColor(0, 0, 0));
    if (mDrawEdges) {
        std::for_each(mEdges.begin(), mEdges.end(),
                      [painter, hF, wF, this](auto e) {
                          if ((mX2 > e->src->x && e->src->x > mX1) && (mX2 > e->dst->x && e->dst->x > mX1)
                              && (mY2 > e->src->y && e->src->y > mY1) && (mY2 > e->dst->y && e->dst->y > mY1)) {
                              painter->drawLine(wF * (e->src->x - mX1), hF * (e->src->y - mY1), wF * (e->dst->x - mX1),
                                                hF * (e->dst->y - mY1));
                          }
                      });
    }
    if (mDrawBox) {
        painter->drawLine(QLineF(wF*(mBoxStartPoint.x()-mX1), hF*(mBoxStartPoint.y()-mY1), wF*(mBoxEndPoint.x()-mX1), hF*(mBoxStartPoint.y()-mY1)));
        painter->drawLine(QLineF(wF*(mBoxEndPoint.x()-mX1), hF*(mBoxStartPoint.y()-mY1), wF*(mBoxEndPoint.x()-mX1), hF*(mBoxEndPoint.y()-mY1)));
        painter->drawLine(QLineF(wF*(mBoxEndPoint.x()-mX1), hF*(mBoxEndPoint.y()-mY1), wF*(mBoxStartPoint.x()-mX1), hF*(mBoxEndPoint.y()-mY1)));
        painter->drawLine(QLineF(wF*(mBoxStartPoint.x()-mX1), hF*(mBoxEndPoint.y()-mY1), wF*(mBoxStartPoint.x()-mX1), hF*(mBoxStartPoint.y()-mY1)));
    }
}

void ProceduralView::updateZoom(QPointF newCentre, qreal zoomFactor)
{
    qreal currWidth = mX2 - mX1;
    qreal currHeight = mY2 - mY1;
    qreal zoomWidth = currWidth * 0.5 / zoomFactor;
    qreal zoomHeight = currHeight * 0.5 / zoomFactor;

    qreal x, y;
    if (zoomFactor < 1) {
        x = currWidth*0.5 + mX1;
        y = currHeight*0.5 + mY1;
    } else {
        x = currWidth*newCentre.x()/mRect.width() + mX1;
        y = currHeight*newCentre.y()/mRect.height() + mY1;

        // Limit offset zooming to what it currently visible
        if (mX1 > x - zoomWidth) x += mX1 - x + zoomWidth;
        if (mX2 < x + zoomWidth) x -= x + zoomWidth - mX2;
        if (mY1 > y - zoomHeight) y += mY1 - y + zoomHeight;
        if (mY2 < y + zoomHeight) y -= y + zoomHeight - mY2;
    }
    mX1 = x - zoomWidth;
    mX2 = x + zoomWidth;
    mY1 = y - zoomHeight;
    mY2 = y + zoomHeight;
    slowUpdate();
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
}

void ProceduralView::applyFocusPositionDelta(QPointF delta)
{
    qreal deltaX = (mX2-mX1)*delta.x()/mRect.width();
    qreal deltaY = (mY2-mY1)*delta.y()/mRect.height();
    auto it = std::partition(mNodes.begin(), mNodes.end(),
                             [](auto n)
                             {
                                 return n->focus == true;
                             });
    std::for_each(mNodes.begin(), it, [deltaX, deltaY](auto n){ n->x = n->x + deltaX; n->y = n->y + deltaY; });
    slowUpdate();
}

QPointF ProceduralView::getFramePos(QPointF scenePos)
{
    return {(mX2-mX1)*scenePos.x()/mRect.width() + mX1, (mY2-mY1)*scenePos.y()/mRect.height() + mY1};
}

void ProceduralView::updateSelectionBoxEnd(QPointF endPoint)
{
    mBoxEndPoint = getFramePos(endPoint);
    qreal x1 = qMin(mBoxStartPoint.x(), mBoxEndPoint.x());
    qreal x2 = qMax(mBoxStartPoint.x(), mBoxEndPoint.x());
    qreal y1 = qMin(mBoxStartPoint.y(), mBoxEndPoint.y());
    qreal y2 = qMax(mBoxStartPoint.y(), mBoxEndPoint.y());
    auto it = std::partition(mNodes.begin(), mIt,
                             [x1, x2, y1, y2](auto n)
                             {
                                 return n->visible && x2 > n->x && n->x > x1 && y2 > n->y && n->y > y1;
                             });
    std::for_each(mNodes.begin(), it, [](auto n){ n->focus = true; });
    std::for_each(it, mNodes.end(), [](auto n){ n->focus = false; });
    slowUpdate();
}

void ProceduralView::selectAllVisible()
{
    auto it = std::partition(mNodes.begin(), mNodes.end(),
                             [this](auto n)
                             {
                                 return mX2 > n->x && n->x > mX1 && mY2 > n->y && n->y > mY1;
                             });
    std::for_each(mNodes.begin(), it, [](auto n){ n->focus = true; });
    std::for_each(it, mNodes.end(), [](auto n){ n->focus = false; });
    slowUpdate();
}

void ProceduralView::toggleDrawDetails()
{
    mDrawDetails = !mDrawDetails;
    update();
}

void ProceduralView::toggleDrawEdges()
{
    mDrawEdges = !mDrawEdges;
    update();
}

void ProceduralView::setRect(QRectF rect)
{
    mRect = rect;
    slowUpdate();
    update();
}
}