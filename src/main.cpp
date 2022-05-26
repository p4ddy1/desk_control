#include <QApplication>
#include <QPushButton>

#include "Gui/MainWindow/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    DeskControl::Gui::MainWindow::MainWindow mainWindow;
    mainWindow.show();
    return QApplication::exec();
}
