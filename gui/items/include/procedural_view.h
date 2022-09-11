#include "items/include/procedural_node.h"

#include <QGraphicsItem>


namespace gui
{
class ProceduralView : public QGraphicsItem {
public:
    explicit ProceduralView(std::vector<ProceduralNode> &nodes);
    ~ProceduralView() override = default;

    /**
     * Find which elements are visible given the current bounding box.
     */
    void slowUpdate();

    /**
     * Same as slowUpdate but assumes all required elements are a subset of
     * the currently visible elements.
     */
    void lessSlowUpdate();

    /**
     * Re-sets the view to the given centre coordinate and applies the
     * given scaling factor.
     * @param newCentre - New coordinates to centre about.
     * @param zoomFactor - Zoom factor. Positive is zoom-in.
     */
    void updateZoom(QPointF newCentre, qreal zoomFactor);

    void applyPositionDelta(QPointF delta);

    void setRect(QRectF rect);

    QRectF boundingRect() const override { return mRect; };
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    std::vector<ProceduralNode> &mNodes;
    __gnu_cxx::__normal_iterator<ProceduralNode*, std::vector<ProceduralNode>> mIt;
    qreal mX1, mX2, mY1, mY2;
    QRectF mRect;
};
}