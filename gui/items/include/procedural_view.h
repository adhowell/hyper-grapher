#include "items/include/procedural_edge.h"

#include <QGraphicsItem>


namespace gui
{
class ProceduralView : public QGraphicsItem {
public:
    explicit ProceduralView(std::vector<ProceduralNode*> &nodes, std::vector<ProceduralEdge*>& edges);
    ~ProceduralView() override = default;

    enum class EdgeDrawMode
    {
        None,
        All, // All edges are drawn unless definitely outside the frame
        One, // Draw the edge if one of the end nodes is in the frame
        Both // Draw the edge if both end nodes are in the frame
    };

    /**
     * Find which elements are visible given the current bounding box.
     */
    void slowUpdate();

    /**
     * Re-sets the view to the given centre coordinate and applies the
     * given scaling factor.
     * @param newCentre - New coordinates to centre about.
     * @param zoomFactor - Zoom factor. >1 is zoom-in.
     */
    void updateZoom(QPointF newCentre, qreal zoomFactor);

    /**
     * Applies the given delta to the frame limits
     */
    void applyPositionDelta(QPointF delta);

    /**
     * Applies the given delta to every currently focused node
     */
    void applyFocusPositionDelta(QPointF delta);

    /**
     * Returns the scene coordinates as frame coordinates, i.e. the mX1, mX2, mY1, mY2 frame
     */
    QPointF getFramePos(QPointF scenePos);

    void drawBox(bool boxVisible) { mDrawBox = boxVisible; }
    void updateSelectionBoxStart(QPointF startPoint) { mBoxStartPoint = getFramePos(startPoint); }
    void updateSelectionBoxEnd(QPointF endPoint);

    /**
     * Highlights every node in the current frame
     */
    void selectAllVisible();

    void deselectAll() { std::for_each(mNodes.begin(), mNodes.end(), [](auto n){ n->focus = false; }); }

    void toggleDrawDetails();

    /**
     * Cycles between the edge-rendering modes. In the current order these are:
     * 1. Render all edges.
     * 2. Render edges with at least one end node in-frame.
     * 3. Render edges with both end nodes in-frame.
     * 4. Don't render any edges.
     */
    void cycleDrawEdges();

    /**
     * Re-sizes the frame according to the window size.
     * TODO: This does not seem very good but I can't think of a better way...
     *
     * @param rect - The size rect of the current window.
     */
    void setRect(QRectF rect);

    /**
     * Changes the current hierarchy level according to the given bool:
     * True: Hides every visible node which has a non-visible non-null parent, and shows the parent.
     * False: Hides every node with a non-visible child, and shows the child.
     */
    void changeHierarchy(bool ascend);

    QRectF boundingRect() const override { return mRect; };
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    bool mDrawDetails = true;
    EdgeDrawMode mEdgeDrawMode = EdgeDrawMode::All;
    bool mDrawBox = false;

    QPointF mBoxStartPoint {0, 0};
    QPointF mBoxEndPoint {0, 0};

    std::vector<ProceduralNode*> &mNodes;
    std::vector<ProceduralEdge*> &mEdges;
    std::vector<ProceduralNode*>::iterator mIt;
    qreal mX1, mX2, mY1, mY2;
    QRectF mRect;
};
}