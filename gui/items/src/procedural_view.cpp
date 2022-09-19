#include "items/include/procedural_view.h"

#include <algorithm>
#include <QPainter>


namespace gui
{
    using PNT = ProceduralNode::Type;

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
                             return n->visible
                             && mX2 > n->x && n->x > mX1 && mY2 > n->y && n->y > mY1;
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
                          switch (n->type) {
                              case PNT::Edge:
                                  painter->drawRect(wF*(n->x-mX1)-5, hF*(n->y-mY1)-5, 10, 10);
                                  break;
                              case PNT::Node:
                                  painter->drawRect(wF*(n->x-mX1)-3, hF*(n->y-mY1)-3, 6, 6);
                                  break;
                              case PNT::Join:
                                  painter->drawRect(wF*(n->x-mX1)-2, hF*(n->y-mY1)-2, 4, 4);
                                  break;
                          }
                      });
    } else {
        std::for_each(mNodes.begin(), mIt,
                      [painter, wF, hF, focusPen, defaultPen, this](auto n) {
                          painter->setPen(n->focus ? focusPen : defaultPen);
                          switch (n->type) {
                              case PNT::Edge:
                              case PNT::Node:
                                  painter->drawLine(wF*(n->x-mX1), hF*(n->y-mY1), wF*(n->x-mX1), hF*(n->y-mY1));
                                  break;
                              case PNT::Join:;
                          }
                      });
    }
    painter->setPen(QColor(0, 0, 0));

    // Ignore hidden edges
    auto nonVisibleIter = std::partition(mEdges.begin(), mEdges.end(),
                                         [](auto e)
                                         {
                                             return e->src->visible && e->dst->visible;
                                         });
    __gnu_cxx::__normal_iterator<ProceduralEdge**, std::vector<ProceduralEdge*>> edgeIter = mEdges.begin();
    switch (mEdgeDrawMode)
    {
        case EdgeDrawMode::All:
            edgeIter = std::partition(mEdges.begin(), nonVisibleIter,
                                      [this](auto e)
                                      {
                                          return !((e->src->x > mX2 && e->dst->x > mX2)
                                                   || (mX1 > e->src->x && mX1 > e->dst->x)
                                                   || (e->src->y > mY2 && e->dst->y > mY2)
                                                   || (mY1 > e->src->y && mY1 > e->dst->y));
                                      });
            break;
        case EdgeDrawMode::One:
            edgeIter = std::partition(mEdges.begin(), nonVisibleIter,
                                      [this](auto e)
                                      {
                                          return ((mX2 > e->src->x && e->src->x > mX1)
                                                  && (mY2 > e->src->y && e->src->y > mY1))
                                                  || ((mX2 > e->dst->x && e->dst->x > mX1)
                                                  && (mY2 > e->dst->y && e->dst->y > mY1));
                                      });
            break;
        case EdgeDrawMode::Both:
            edgeIter = std::partition(mEdges.begin(), nonVisibleIter,
                                      [this](auto e)
                                      {
                                          return (mX2 > e->src->x && e->src->x > mX1)
                                                 && (mY2 > e->src->y && e->src->y > mY1)
                                                 && (mX2 > e->dst->x && e->dst->x > mX1)
                                                 && (mY2 > e->dst->y && e->dst->y > mY1);
                                      });
            break;
        default: break;
    }
    std::for_each(mEdges.begin(), edgeIter,
                  [painter, hF, wF, this](auto e) {
                      painter->drawLine(wF * (e->src->x - mX1),
                                        hF * (e->src->y - mY1),
                                        wF * (e->dst->x - mX1),
                                        hF * (e->dst->y - mY1));
                  });
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
    QPointF frameDelta = {(mX2-mX1)*delta.x()/mRect.width(), (mY2-mY1)*delta.y()/mRect.height()};
    auto it = std::partition(mNodes.begin(), mNodes.end(),
                             [](auto n)
                             {
                                 return n->focus == true;
                             });
    std::for_each(mNodes.begin(), it,
                  [frameDelta](auto n)
                  {
                      n->applyPositionDelta(frameDelta);
                  });
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
    std::for_each(mNodes.begin(), it, [](auto n){ n->setFocus(true); });
    std::for_each(it, mNodes.end(), [](auto n){ n->setFocus(false); });
    slowUpdate();
}

void ProceduralView::selectAllVisible()
{
    auto it = std::partition(mNodes.begin(), mNodes.end(),
                             [this](auto n)
                             {
                                 return mX2 > n->x && n->x > mX1 && mY2 > n->y && n->y > mY1;
                             });
    std::for_each(mNodes.begin(), it, [](auto n){ n->setFocus(true); });
    std::for_each(it, mNodes.end(), [](auto n){ n->setFocus(false); });
    slowUpdate();
}

void ProceduralView::toggleDrawDetails()
{
    mDrawDetails = !mDrawDetails;
    update();
}

void ProceduralView::cycleDrawEdges()
{
    switch (mEdgeDrawMode)
    {
        case EdgeDrawMode::All: mEdgeDrawMode = EdgeDrawMode::One; break;
        case EdgeDrawMode::One: mEdgeDrawMode = EdgeDrawMode::Both; break;
        case EdgeDrawMode::Both: mEdgeDrawMode = EdgeDrawMode::None; break;
        case EdgeDrawMode::None: mEdgeDrawMode = EdgeDrawMode::All; break;
    }
    update();
}

void ProceduralView::setRect(QRectF rect)
{
    mRect = rect;
    slowUpdate();
    update();
}

void ProceduralView::changeHierarchy(bool ascend)
{
    // TODO: Shift this to another thread?
    if (ascend) {
        std::unordered_map<ProceduralNode*, QPointF> nodeAccumPos;
        std::unordered_map<ProceduralNode*, double> nodeSum;
        auto it = std::partition(mNodes.begin(), mNodes.end(),
                                 [&nodeAccumPos, &nodeSum](auto n)
                                 {
                                     bool hasVisibleParent = false;
                                     for (auto p : n->parents) {
                                         nodeAccumPos[p] += n->getPos();
                                         nodeSum[p]++;
                                         if (p->visible) {
                                             hasVisibleParent = true;
                                         }
                                     }
                                     return n->visible && !n->parents.empty() && !hasVisibleParent;
                                 });
        std::for_each(mNodes.begin(), it,
                      [&nodeAccumPos, &nodeSum](auto n)
                      {
                          n->setVisible(false);
                          for (auto p : n->parents) {
                              p->setVisible(true);
                              p->setPos(nodeAccumPos[p]/nodeSum[p]);
                          }
                      });
    } else {
        auto it = std::partition(mNodes.begin(), mNodes.end(),
                                 [](auto n)
                                 {
                                     return !n->visible && std::any_of(n->parents.begin(), n->parents.end(),
                                                                       [](auto p)
                                                                       {
                                                                           return p->visible;
                                                                       });
                                 });
        std::for_each(mNodes.begin(), it,
                      [](auto n)
                      {
                          n->setVisible(true);
                          QPointF accumPos {};
                          for (auto p : n->parents) {
                              accumPos += p->getPos();
                              p->setVisible(false);
                          }
                          n->setPos(accumPos/n->parents.size());
                      });
    }
    slowUpdate();
}
}