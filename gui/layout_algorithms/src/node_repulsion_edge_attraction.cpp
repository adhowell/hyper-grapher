#include "layout_algorithms/include/node_repulsion_edge_attraction.h"

#include <QPointF>


namespace gui
{
    void NodeRepulsionEdgeAttraction::calculateNext()
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

        for (auto itI = mNodes.begin(); itI != mNodes.end(); itI++) {
            for (auto itJ = itI; itJ != mNodes.end(); itJ++) {
                double deltaX = (*itI)->x - (*itJ)->x;
                double deltaY = (*itI)->y - (*itJ)->y;
                double hypot = qSqrt(deltaX*deltaX + deltaY*deltaY);
                double force = qMin(100.0, 1.0 / (hypot*hypot));

                // Fast but leads to Inf/NaN values
                (*itI)->schedulePositionDelta({deltaX*force, deltaY*force});
                (*itJ)->schedulePositionDelta({-deltaX*force, -deltaY*force});
            }
        }
        std::for_each(mNodes.begin(), mNodes.end(),
                      [](auto n)
                      {
                          n->applyScheduledPositionDelta();
                      });
    }
}