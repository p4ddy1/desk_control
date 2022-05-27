// You may need to build the project (run Qt uic code generator) to get "ui_DeviceScanWindow.h" resolved

#include "DeviceScanWindow.h"
#include "ui_DeviceScanWindow.h"

namespace DeskControl::Gui::DeviceScanWindow {
    DeviceScanWindow::DeviceScanWindow(QWidget *parent) :
            QDialog(parent), ui(new Ui::DeviceScanWindow) {
        ui->setupUi(this);

        configStorage = new DeviceConfigStorage();

        deskModel = new DeskModel(this);
        ui->devicesListView->setModel(deskModel);

        bluetoothScanner = new BluetoothScanner(20000, this);

        connect(bluetoothScanner, &BluetoothScanner::deskFound, this, &DeviceScanWindow::deskFound);
        connect(bluetoothScanner, &BluetoothScanner::scanFinished, this, &DeviceScanWindow::scanFinished);

        connect(ui->startButton, &QPushButton::clicked, this, &DeviceScanWindow::startButtonClicked);
        connect(ui->stopButton, &QPushButton::clicked, this, &DeviceScanWindow::stopButtonClicked);
        connect(ui->addButton, &QPushButton::clicked, this, &DeviceScanWindow::addButtonClicked);
    }

    DeviceScanWindow::~DeviceScanWindow() {
        delete ui;
    }

    void DeviceScanWindow::deskFound(Desk* desk) {
        deskModel->add(desk);
    }

    void DeviceScanWindow::startButtonClicked() {
        auto startButton = findChild<QPushButton*>("startButton");
        auto stopButton = findChild<QPushButton*>("stopButton");

        bluetoothScanner->startScan();

        startButton->setEnabled(false);
        stopButton->setEnabled(true);
    }

    void DeviceScanWindow::stopButtonClicked() {
        auto startButton = findChild<QPushButton*>("startButton");
        auto stopButton = findChild<QPushButton*>("stopButton");

        bluetoothScanner->stopScan();

        startButton->setEnabled(true);
        stopButton->setEnabled(false);

        setAddButtonState();
    }

    void DeviceScanWindow::addButtonClicked() {
        auto deviceListView = findChild<QListView*>("devicesListView");

        auto selectedDesk = deskModel->get(deviceListView->currentIndex());

        auto deskList = configStorage->load();
        deskList.append(selectedDesk);

        configStorage->save(deskList);

        emit deviceConfigChanged();

        this->close();
    }

    void DeviceScanWindow::setAddButtonState() {
        auto addButton = findChild<QPushButton*>("addButton");

        if (deskModel->rowCount(QModelIndex()) <= 0) {
            addButton->setEnabled(false);
        } else {
            addButton->setEnabled(true);
        }
    }

    void DeviceScanWindow::scanFinished() {
        setAddButtonState();
    }
} // DeskControl::Gui::DeviceScanWindow
