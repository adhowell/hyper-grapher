#include <QtMath>
#include <QPointF>
#include <mutex>


namespace gui
{
struct ProceduralNode {
    double x;
    double y;
    bool visible = false;
    bool focus = false;

    std::mutex m;

    void applyPositionDelta(QPointF delta)
    {
        std::lock_guard<std::mutex> guard(m);
        x += delta.x();
        y += delta.y();
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