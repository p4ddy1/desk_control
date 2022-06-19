#ifndef TRAYPOSITIONLISTVIEW_H
#define TRAYPOSITIONLISTVIEW_H

#include <QListView>

namespace DeskControl::Gui::MainWindow
{

class TrayPositionListView: public QListView
{
Q_OBJECT
public:
    explicit TrayPositionListView(QWidget *parent = nullptr);

    QSize sizeHint() const override;

private:
    int maximumVisibleItems;
};

} // MainWindow

#endif //TRAYPOSITIONLISTVIEW_H
