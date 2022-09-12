#include <QtMath>
#include <QtDebug>


namespace gui
{
struct ProceduralNode {
    std::atomic<double> x;
    std::atomic<double> y;
    std::atomic<bool> visible = false;
    std::atomic<bool> focus = false;
};
}