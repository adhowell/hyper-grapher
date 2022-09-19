#include <QtMath>
#include <QPointF>
#include <mutex>

#pragma once


namespace gui
{
struct ProceduralNode {
    double x;
    double y;
    bool visible = false;
    ProceduralNode* parent = nullptr;
    bool focus = false;
    double _scheduledDeltaX = 0;
    double _scheduledDeltaY = 0;

    std::mutex m;

    QPointF getPos()
    {
        std::lock_guard<std::mutex> guard(m);
        return {x, y};
    }

    void setPos(QPointF pos)
    {
        std::lock_guard<std::mutex> guard(m);
        x = pos.x();
        y = pos.y();
    }

    void applyPositionDelta(QPointF delta)
    {
        std::lock_guard<std::mutex> guard(m);
        x += delta.x();
        y += delta.y();
    }

    void schedulePositionDelta(QPointF delta)
    {
        std::lock_guard<std::mutex> guard(m);
        _scheduledDeltaX += delta.x();
        _scheduledDeltaY += delta.y();
    }

    void applyScheduledPositionDelta()
    {
        std::lock_guard<std::mutex> guard(m);
        x += _scheduledDeltaX;
        y += _scheduledDeltaY;
        _scheduledDeltaX = 0.0;
        _scheduledDeltaY = 0.0;
    }

    void setVisible(bool b)
    {
        std::lock_guard<std::mutex> guard(m);
        visible = b;
    }

    void setFocus(bool b)
    {
        std::lock_guard<std::mutex> guard(m);
        focus = b;
    }
};
}