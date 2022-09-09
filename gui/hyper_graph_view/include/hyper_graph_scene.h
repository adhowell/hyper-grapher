#include "hyper_graph_view.h"
#include "model/include/hyper_graph.h"

#include <QGraphicsScene>

#pragma once


namespace gui
{
class HyperGraphScene : public QGraphicsScene
{
Q_OBJECT
public:
    explicit HyperGraphScene(core::HyperGraph& graph);
    ~HyperGraphScene() override = default;

    [[nodiscard]] gui::HyperGraphView* getView() const { return mView; }

private:
    gui::HyperGraphView* mView;
};
}