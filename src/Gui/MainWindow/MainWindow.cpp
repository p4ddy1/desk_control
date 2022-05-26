// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>

namespace DeskControl::Gui::MainWindow {
    MainWindow::MainWindow(QWidget *parent) :
            QWidget(parent), ui(new Ui::MainWindow) {
        ui->setupUi(this);

        auto heightMapping = new Bluetooth::Service::HeightMapping{1760, 800};

        bluetoothController = new BluetoothController(heightMapping, this);

        connect(bluetoothController, &BluetoothController::connected, this, &MainWindow::connected);
        connect(bluetoothController, &BluetoothController::connectionFailed, this, &MainWindow::connectionFailed);
        connect(bluetoothController, &BluetoothController::heightChanged, this, &MainWindow::heightChanged);

        deskModel = new DeskModel(this);
        //This is only temporary until scanning is implemented
        deskModel->add(new Bluetooth::Model::Desk(
                QBluetoothUuid("{foo}"),
                "Desk 8113",
                0
                ));

        ui->deviceComboBox->setModel(deskModel);


        connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectButtonClicked);
        connect(ui->upButton, &QPushButton::clicked, this, &MainWindow::upButtonClicked);
        connect(ui->downButton, &QPushButton::clicked, this, &MainWindow::downButtonClicked);
    }

    MainWindow::~MainWindow() {
        delete ui;
    }

    void MainWindow::connectButtonClicked() {
        auto deviceComboBox = findChild<QComboBox*>("deviceComboBox");

        auto selectedDesk = deskModel->get(deviceComboBox->currentIndex());

        bluetoothController->connectToDesk(selectedDesk);
    }

    void MainWindow::upButtonClicked() {
        bluetoothController->move(Bluetooth::Service::Direction::Up);
    }

    void MainWindow::downButtonClicked() {
        bluetoothController->move(Bluetooth::Service::Direction::Down);
    }

    void MainWindow::connected() {
        auto upButton = findChild<QPushButton*>("upButton");
        auto downButton = findChild<QPushButton*>("downButton");
        auto disconnectButton = findChild<QPushButton*>("disconnectButton");
        auto connectButton = findChild<QPushButton*>("connectButton");
        auto scanButton = findChild<QPushButton*>("scanButton");

        upButton->setEnabled(true);
        downButton->setEnabled(true);
        disconnectButton->setEnabled(true);
        connectButton->setEnabled(false);
        scanButton->setEnabled(false);
    }

    void MainWindow::connectionFailed(QString errorMessage) {
        QMessageBox msgBox;
        msgBox.setText("Failed to connect: " + errorMessage);
        msgBox.exec();
    }

    void MainWindow::heightChanged(int heightInMm) {
        auto heightLabel = findChild<QLabel*>("heightLabel");

        int heightInCm = heightInMm / 10;

        heightLabel->setText(QString::number(heightInCm) + " cm");
    }
} // DeskControl::Gui::MainWindow