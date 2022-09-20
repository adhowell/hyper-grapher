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

                          double xForce = deltaX*force;
                          double yForce = deltaY*force;
                          auto normFactor = qMax(qAbs(xForce)/100.0, qAbs(yForce)/100.0);
                          if (normFactor > 1.0) {
                              xForce /= normFactor;
                              yForce /= normFactor;
                          }
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