#include "TrayPositionListView.h"

namespace DeskControl::Gui::MainWindow
{
TrayPositionListView::TrayPositionListView(QWidget *parent)
    : QListView(parent), maximumVisibleItems(20)
{

}
QSize TrayPositionListView::sizeHint() const
{
    if (model()->rowCount() == 0) {
        return QSize(QWidget::width(), 0);
    }

    int itemsToShow = std::min(model()->rowCount(), maximumVisibleItems);

    return QSize(QWidget::width(), itemsToShow * (sizeHintForRow(0) + spacing()));
}
} // MainWindow