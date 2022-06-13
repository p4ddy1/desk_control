#include <QApplication>
#include <QPushButton>

#include "Gui/MainWindow/MainWindow.h"
#include "Config/ConfigStorage.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto configStorage = new DeskControl::Config::ConfigStorage("config.json");
    auto config = configStorage->load();

    DeskControl::Gui::MainWindow::MainWindow mainWindow(configStorage, config);
    mainWindow.show();
    return QApplication::exec();
}
