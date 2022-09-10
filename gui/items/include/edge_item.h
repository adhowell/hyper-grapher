#include "node_item.h"


namespace gui
{
class EdgeItem : public QGraphicsItem
{
public:
    EdgeItem(NodeItem* src, NodeItem* dst) : QGraphicsItem(), mSrc(src), mDst(dst) {}
    ~EdgeItem() override = default;

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    NodeItem* mSrc;
    NodeItem* mDst;
};
}