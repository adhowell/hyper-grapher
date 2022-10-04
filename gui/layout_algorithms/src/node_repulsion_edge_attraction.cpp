#include "layout_algorithms/include/node_repulsion_edge_attraction.h"

#include <QPointF>


namespace gui
{
    void NodeRepulsionEdgeAttraction::calculateNext()
    {
        auto edgeIt = std::partition(mEdges.begin(), mEdges.end(),
                                 [](auto e)
                                 {
                                     return e->src->visible && e->dst->visible;
                                 });
        std::for_each(mEdges.begin(), edgeIt,
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

        auto nodeIt = std::partition(mNodes.begin(), mNodes.end(),
                                     [](auto n)
                                     {
                                         return n->visible;
                                     });
        for (auto itI = mNodes.begin(); itI != nodeIt; itI++) {
            for (auto itJ = itI+1; itJ != nodeIt; itJ++) {
                double deltaX = (*itI)->x - (*itJ)->x;
                double deltaY = (*itI)->y - (*itJ)->y;
                double hypot = qSqrt(deltaX*deltaX + deltaY*deltaY);
                double force = qMin(100.0, 1.0 / (hypot*hypot));

                double xForce = deltaX*force;
                double yForce = deltaY*force;
                auto normFactor = qMax(qAbs(xForce)/100.0, qAbs(yForce)/100.0);
                if (normFactor > 1.0) {
                    xForce /= normFactor;
                    yForce /= normFactor;
                }
                (*itI)->schedulePositionDelta({xForce, yForce});
                (*itJ)->schedulePositionDelta({-xForce, -yForce});
            }
        }
        std::for_each(mNodes.begin(), mNodes.end(),
                      [](auto n)
                      {
                          n->applyScheduledPositionDelta();
                      });
    }
}