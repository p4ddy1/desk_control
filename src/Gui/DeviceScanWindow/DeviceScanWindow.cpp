// You may need to build the project (run Qt uic code generator) to get "ui_DeviceScanWindow.h" resolved

#include "DeviceScanWindow.h"
#include "ui_DeviceScanWindow.h"

namespace DeskControl::Gui::DeviceScanWindow {
    DeviceScanWindow::DeviceScanWindow(QWidget *parent) :
            QDialog(parent), ui(new Ui::DeviceScanWindow) {
        ui->setupUi(this);
    }

    DeviceScanWindow::~DeviceScanWindow() {
        delete ui;
    }
} // DeskControl::Gui::DeviceScanWindow
