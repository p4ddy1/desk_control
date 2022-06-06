// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include "../DeviceScanWindow/DeviceScanWindow.h"

namespace DeskControl::Gui::MainWindow {
    using DeviceScanWindow::DeviceScanWindow;

    MainWindow::MainWindow(ConfigStorage* configStorage, Config* config, QWidget *parent) :
            config(config), configStorage(configStorage),
            QWidget(parent), ui(new Ui::MainWindow) {
        ui->setupUi(this);

        auto heightMapping = new Bluetooth::Service::HeightMapping{1760, 800};

        bluetoothController = new BluetoothController(heightMapping, this);

        connect(bluetoothController, &BluetoothController::connected, this, &MainWindow::connected);
        connect(bluetoothController, &BluetoothController::disconnected, this, &MainWindow::disconnected);
        connect(bluetoothController, &BluetoothController::connectionFailed, this, &MainWindow::connectionFailed);
        connect(bluetoothController, &BluetoothController::heightChanged, this, &MainWindow::heightChanged);

        targetHeightMovementService = new TargetHeightMovementService(bluetoothController, this);

        positionModel = new PositionModel(this);

        ui->storedPositionTable->setModel(positionModel);

        deskModel = new DeskModel(this);

        ui->deviceComboBox->setModel(deskModel);

        connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectButtonClicked);
        connect(ui->disconnectButton, &QPushButton::clicked, this, &MainWindow::disconnectButtonClicked);
        connect(ui->upButton, &QPushButton::clicked, this, &MainWindow::upButtonClicked);
        connect(ui->downButton, &QPushButton::clicked, this, &MainWindow::downButtonClicked);
        connect(ui->scanButton, &QPushButton::clicked, this, &MainWindow::scanButtonClicked);
        connect(ui->addCurrentPositionButton, &QPushButton::clicked, this, &MainWindow::addCurrentPositionButtonClicked);
        connect(ui->deletePositionButton, &QPushButton::clicked, this, &MainWindow::deletePositionButtonClicked);
        connect(ui->moveToPositionButton, &QPushButton::clicked, this, &MainWindow::moveToPositionButtonClicked);
        connect(ui->addPositionInputButton, &QPushButton::clicked, this, &MainWindow::addPositionInputButtonClicked);
    }

    MainWindow::~MainWindow() {
        delete ui;
    }

    void MainWindow::connectButtonClicked() {
        auto deviceComboBox = findChild<QComboBox*>("deviceComboBox");

        auto selectedDesk = deskModel->get(deviceComboBox->currentIndex());

        bluetoothController->connectToDesk(selectedDesk);
    }

    void MainWindow::disconnectButtonClicked() {
        bluetoothController->disconnectFromDesk();
    }

    void MainWindow::upButtonClicked() {
        bluetoothController->move(Bluetooth::Service::Direction::Up);
    }

    void MainWindow::downButtonClicked() {
        bluetoothController->move(Bluetooth::Service::Direction::Down);
    }

    void MainWindow::connected() {
        auto disconnectButton = findChild<QPushButton*>("disconnectButton");
        auto connectButton = findChild<QPushButton*>("connectButton");
        auto scanButton = findChild<QPushButton*>("scanButton");
        auto controlGroupBox = findChild<QGroupBox*>("controlGroupBox");
        auto storedPositionsGroupBox = findChild<QGroupBox*>("storedPositionsGroupBox");

        controlGroupBox->setEnabled(true);
        storedPositionsGroupBox->setEnabled(true);
        disconnectButton->setEnabled(true);
        connectButton->setEnabled(false);
        scanButton->setEnabled(false);
    }

    void MainWindow::disconnected() {
        auto disconnectButton = findChild<QPushButton*>("disconnectButton");
        auto connectButton = findChild<QPushButton*>("connectButton");
        auto scanButton = findChild<QPushButton*>("scanButton");
        auto controlGroupBox = findChild<QGroupBox*>("controlGroupBox");
        auto storedPositionsGroupBox = findChild<QGroupBox*>("storedPositionsGroupBox");

        controlGroupBox->setEnabled(false);
        storedPositionsGroupBox->setEnabled(false);
        disconnectButton->setEnabled(false);
        connectButton->setEnabled(true);
        scanButton->setEnabled(true);
    }

    void MainWindow::connectionFailed(QString errorMessage) {
        QMessageBox msgBox;
        msgBox.setText("Failed to connect: " + errorMessage);
        msgBox.exec();
    }

    void MainWindow::heightChanged(int heightInMm) {
        currentHeightMm = heightInMm;

        auto heightLabel = findChild<QLabel*>("heightLabel");

        int heightInCm = heightInMm / 10;

        heightLabel->setText(QString::number(heightInCm) + " cm");
    }

    void MainWindow::scanButtonClicked() {
        DeviceScanWindow deviceScanWindow(this);

        connect(&deviceScanWindow, &DeviceScanWindow::deviceConfigChanged, this, &MainWindow::deviceConfigChanged);

        deviceScanWindow.exec();
    }

    void MainWindow::showEvent(QShowEvent *event) {
        loadDeviceList();
        loadPositionList();
    }

    void MainWindow::loadDeviceList() {
        auto deskList = config->getDeskList();

        deskModel->clear();
        deskModel->add(deskList);

        auto deviceComboBox = findChild<QComboBox*>("deviceComboBox");

        if (!deskList.empty()) {
            deviceComboBox->setCurrentIndex(0);
        }
    }

    void MainWindow::loadPositionList() {
        auto positionList = config->getPositionList();

        positionModel->clear();
        positionModel->add(positionList);
    }

    void MainWindow::deviceConfigChanged(Desk* newDesk) {
        deskModel->add(newDesk);

        config->setDeskList(deskModel->getDeskList());
        configStorage->save(config);

        loadDeviceList();
    }

    void MainWindow::addCurrentPositionButtonClicked() {
        auto name = askForPositionName();

        if (name == "") {
            return;
        }

        auto position = new Model::Position(name, currentHeightMm);
        positionModel->add(position);
        savePositionList();
    }

    QString MainWindow::askForPositionName() {
        auto text = QInputDialog::getText(
                this,
                "Insert name",
                "Please insert a name for the position",
                QLineEdit::Normal,
                ""
                );

        return text;
    }

    void MainWindow::savePositionList() {
        config->setPositionList(positionModel->getPositionList());
        configStorage->save(config);
    }

    void MainWindow::deletePositionButtonClicked() {
        auto storedPositionTable = findChild<QTableView*>("storedPositionTable");

        positionModel->remove(storedPositionTable->currentIndex());
        savePositionList();
    }

    void MainWindow::moveToPositionButtonClicked() {
        auto storedPositionTable = findChild<QTableView*>("storedPositionTable");

        auto index = storedPositionTable->currentIndex();

        if (!index.isValid()) {
            return;
        }

        auto position = positionModel->get(index);
        targetHeightMovementService->moveToHeight(position->getHeightMm());
    }

    void MainWindow::addPositionInputButtonClicked() {
        auto positionInput = findChild<QLineEdit*>("positionInput");

        auto position = positionInput->text().replace(" cm", "").toInt() * 10;

        auto name = askForPositionName();

        if (name == "") {
            return;
        }

        positionModel->add(new Model::Position(name, position));
        savePositionList();
    }
} // DeskControl::Gui::MainWindow
