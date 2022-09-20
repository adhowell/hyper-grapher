#include "hyper_graph_view/include/hyper_graph_scene.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QLabel>


namespace gui
{
class MainWindow : public QWidget {
Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

    void resizeEvent(QResizeEvent *event) override;

public Q_SLOTS:
    void handleUpdateNumSelected(int numSelected);

private:
    gui::HyperGraphScene* mScene;
    QLabel* mNumSelectedLabel;
};
}