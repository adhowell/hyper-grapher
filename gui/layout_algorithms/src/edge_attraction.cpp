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
                          double force = mAttraction * (qSqrt(deltaX*deltaX + deltaY*deltaY) - mRestingDistance);

                          double xForce = std::clamp(deltaX*force, -100.0, 100.0);
                          double yForce = std::clamp(deltaY*force, -100.0, 100.0);
                          e->src->schedulePositionDelta({xForce, yForce});
                          e->dst->schedulePositionDelta({-xForce, -yForce});
                      });

        std::for_each(mNodes.begin(), mNodes.end(),
                      [](auto n)
                      {
                          n->applyScheduledPositionDelta();
                      });
    }
}