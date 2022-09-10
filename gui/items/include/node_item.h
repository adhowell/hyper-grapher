#include <QGraphicsItem>
#include <QPen>


namespace gui
{
    class NodeItem : public QGraphicsItem
    {
    public:
        explicit NodeItem(NodeItem* parent = nullptr);
        ~NodeItem() override = default;

        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

        void addChild(NodeItem* node);
        void addEdge(QGraphicsItem* edge);

        std::vector<QPointF> getNodePositions();

        QRectF boundingRect() const override;
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    private:
        NodeItem* mParent = nullptr;
        std::vector<NodeItem*> mChildren;
        QGraphicsItem* mOutEdge = nullptr;

        qreal mRadius = 5;
        QPen mPen;
        QBrush mBrush;
    };
}