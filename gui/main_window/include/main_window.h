#include "hyper_graph_view/include/hyper_graph_scene.h"

#include <QWidget>
#include <QGraphicsScene>


namespace gui
{
class MainWindow : public QWidget {
Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private:
    gui::HyperGraphScene* mScene;
};
}