#include "layout_algorithms/include/edge_attraction.h"

#include <QPointF>


namespace gui
{
    void EdgeAttraction::calculateNext()
    {
        auto it = std::partition(mEdges.begin(), mEdges.end(),
                                 [](auto e)
                                 {
                                     return e->src->visible && e->dst->visible;
                                 });
        std::for_each(mEdges.begin(), it,
                      [this](auto e)
                      {
                          double deltaX = e->dst->x - e->src->x;
                          double deltaY = e->dst->y - e->src->y;
                          double force = qMin(100.0, mAttraction * (qSqrt(deltaX*deltaX + deltaY*deltaY) - mRestingDistance));

                          // Fast but leads to Inf/NaN values
                          e->src->schedulePositionDelta({deltaX*force, deltaY*force});
                          e->dst->schedulePositionDelta({-deltaX*force, -deltaY*force});
                      });

        std::for_each(mNodes.begin(), mNodes.end(),
                      [](auto n)
                      {
                          n->applyScheduledPositionDelta();
                      });
    }
}