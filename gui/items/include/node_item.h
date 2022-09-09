#include <QGraphicsItem>


namespace gui
{
    class NodeItem : public QGraphicsItem
    {
    public:
        NodeItem() : QGraphicsItem() {}
        ~NodeItem() override = default;

        QRectF boundingRect() const override;
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    private:
        qreal mRadius = 5;
    };
}