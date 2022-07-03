// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include "../DeviceScanWindow/DeviceScanWindow.h"
#include "../Model/Position.h"

namespace DeskControl::Gui::MainWindow
{
using DeviceScanWindow::DeviceScanWindow;
using Model::Position;

MainWindow::MainWindow(ConfigStorage *configStorage, Config::Model::Config *config, QWidget *parent)
    : config(config), configStorage(configStorage), QWidget(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createTrayIcon();
    createMainMenu();

    bluetoothController = new BluetoothController(config->getHeightMapping(), this);

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
    connect(ui->addCurrentPositionButton, &QPushButton::clicked, this,
            &MainWindow::addCurrentPositionButtonClicked);
    connect(ui->deletePositionButton, &QPushButton::clicked, this, &MainWindow::deletePositionButtonClicked);
    connect(ui->moveToPositionButton, &QPushButton::clicked, this, &MainWindow::moveToPositionButtonClicked);
    connect(ui->addPositionInputButton, &QPushButton::clicked, this, &MainWindow::addPositionInputButtonClicked);

    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &MainWindow::aboutToQuit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectButtonClicked()
{
    auto deviceComboBox = findChild<QComboBox *>("deviceComboBox");

    if (deskModel->rowCount(QModelIndex()) <= 0) {
        return;
    }

    auto selectedDesk = deskModel->get(deviceComboBox->currentIndex());

    bluetoothController->connectToDesk(selectedDesk);
}

void MainWindow::disconnectButtonClicked()
{
    bluetoothController->disconnectFromDesk();
}

void MainWindow::upButtonClicked()
{
    bluetoothController->move(Bluetooth::Service::Direction::Up);
}

void MainWindow::downButtonClicked()
{
    bluetoothController->move(Bluetooth::Service::Direction::Down);
}

void MainWindow::connected()
{
    auto disconnectButton = findChild<QPushButton *>("disconnectButton");
    auto connectButton = findChild<QPushButton *>("connectButton");
    auto scanButton = findChild<QPushButton *>("scanButton");
    auto controlGroupBox = findChild<QGroupBox *>("controlGroupBox");
    auto storedPositionsGroupBox = findChild<QGroupBox *>("storedPositionsGroupBox");

    controlGroupBox->setEnabled(true);
    storedPositionsGroupBox->setEnabled(true);
    disconnectButton->setEnabled(true);
    connectButton->setEnabled(false);
    scanButton->setEnabled(false);
    upAction->setEnabled(true);
    downAction->setEnabled(true);
    trayPositionMenu->setEnabled(true);
    configureHeightAction->setEnabled(true);
}

void MainWindow::disconnected()
{
    auto disconnectButton = findChild<QPushButton *>("disconnectButton");
    auto connectButton = findChild<QPushButton *>("connectButton");
    auto scanButton = findChild<QPushButton *>("scanButton");
    auto controlGroupBox = findChild<QGroupBox *>("controlGroupBox");
    auto storedPositionsGroupBox = findChild<QGroupBox *>("storedPositionsGroupBox");

    controlGroupBox->setEnabled(false);
    storedPositionsGroupBox->setEnabled(false);
    disconnectButton->setEnabled(false);
    connectButton->setEnabled(true);
    scanButton->setEnabled(true);
    upAction->setEnabled(false);
    downAction->setEnabled(false);
    trayPositionMenu->setEnabled(false);
    configureHeightAction->setEnabled(false);
}

void MainWindow::connectionFailed(QString errorMessage)
{
    QMessageBox msgBox;
    msgBox.setText("Failed to connect: " + errorMessage);
    msgBox.exec();
}

void MainWindow::heightChanged(int heightInMm)
{
    currentHeightMm = heightInMm;

    auto heightLabel = findChild<QLabel *>("heightLabel");

    int heightInCm = heightInMm / 10;

    heightLabel->setText(QString::number(heightInCm) + " cm");
}

void MainWindow::scanButtonClicked()
{
    DeviceScanWindow deviceScanWindow(this);

    connect(&deviceScanWindow, &DeviceScanWindow::deviceConfigChanged, this, &MainWindow::deviceConfigChanged);

    deviceScanWindow.exec();
}

void MainWindow::showEvent(QShowEvent *event)
{
    loadDeviceList();
    loadPositionList();
}

void MainWindow::loadDeviceList()
{
    auto deskList = config->getDeskList();

    deskModel->clear();
    deskModel->add(deskList);

    auto deviceComboBox = findChild<QComboBox *>("deviceComboBox");

    if (!deskList.empty()) {
        deviceComboBox->setCurrentIndex(0);
    }
}

void MainWindow::loadPositionList()
{
    auto positionList = config->getPositionList();

    positionModel->clear();
    positionModel->add(positionList);
}

void MainWindow::deviceConfigChanged(Desk *newDesk)
{
    deskModel->add(newDesk);

    config->setDeskList(deskModel->getDeskList());
    configStorage->save(config);

    loadDeviceList();
}

void MainWindow::addCurrentPositionButtonClicked()
{
    auto name = askForPositionName();

    if (name == "") {
        return;
    }

    auto position = new Model::Position(name, currentHeightMm);
    positionModel->add(position);
    saveConfig();
}

QString MainWindow::askForPositionName()
{
    auto text = QInputDialog::getText(
        this,
        "Insert name",
        "Please insert a name for the position",
        QLineEdit::Normal,
        ""
    );

    return text;
}

void MainWindow::saveConfig()
{
    config->setPositionList(positionModel->getPositionList());
    configStorage->save(config);
}

void MainWindow::deletePositionButtonClicked()
{
    auto storedPositionTable = findChild<QTableView *>("storedPositionTable");

    positionModel->remove(storedPositionTable->currentIndex());
    saveConfig();
}

void MainWindow::moveToPositionButtonClicked()
{
    auto storedPositionTable = findChild<QTableView *>("storedPositionTable");

    auto index = storedPositionTable->currentIndex();

    if (!index.isValid()) {
        return;
    }

    auto position = positionModel->get(index);
    targetHeightMovementService->moveToHeight(position->getHeightMm());
}

void MainWindow::addPositionInputButtonClicked()
{
    auto positionInput = findChild<QLineEdit *>("positionInput");

    auto position = positionInput->text().replace(" cm", "").toInt() * 10;

    auto name = askForPositionName();

    if (name == "") {
        return;
    }

    positionModel->add(new Model::Position(name, position));
    saveConfig();
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(showAction);
    trayIconMenu->addAction(hideAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(upAction);
    trayIconMenu->addAction(downAction);
    trayIconMenu->addSeparator();

    trayPositionMenu = trayIconMenu->addMenu("Stored Positions");
    trayPositionMenu->setEnabled(false);

    connect(trayPositionMenu, &QMenu::aboutToShow, this, &MainWindow::trayPositionMenuAboutToShow);

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    QIcon icon(":/images/tray.ico");
    trayIcon->setIcon(icon);

    trayIcon->show();
}

void MainWindow::moveToPositionTrayClicked()
{
    auto sender = qobject_cast<QAction *>(QObject::sender());
    auto position = (Position *) sender->data().value<void *>();


    targetHeightMovementService->moveToHeight(position->getHeightMm());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void MainWindow::trayPositionMenuAboutToShow()
{
    trayPositionMenu->clear();

    for (auto position : positionModel->getPositionList()) {
        auto action = new QAction(position->getName(), trayPositionMenu);
        auto data = QVariant::fromValue((void *) position);
        action->setData(data);

        connect(action, &QAction::triggered, this, &MainWindow::moveToPositionTrayClicked);

        trayPositionMenu->addAction(action);
    }
}

void MainWindow::aboutToQuit()
{
    saveConfig();
}

void MainWindow::createMainMenu()
{
    mainMenu = new QMenuBar(this);
    auto appMenu = mainMenu->addMenu("Desk");

    appMenu->addAction(upAction);
    appMenu->addAction(downAction);
    appMenu->addSeparator();
    appMenu->addAction(configureHeightAction);
    appMenu->addSeparator();
    appMenu->addAction(quitAction);

    layout()->setMenuBar(mainMenu);
}
void MainWindow::createActions()
{
    hideAction = new QAction("Hide", this);
    connect(hideAction, &QAction::triggered, this, &MainWindow::hide);

    showAction = new QAction("Show", this);
    connect(showAction, &QAction::triggered, this, &MainWindow::show);

    upAction = new QAction("Up", this);
    connect(upAction, &QAction::triggered, this, &MainWindow::upButtonClicked);
    upAction->setEnabled(false);

    downAction = new QAction("Down", this);
    connect(downAction, &QAction::triggered, this, &MainWindow::downButtonClicked);
    downAction->setEnabled(false);

    quitAction = new QAction("Quit", this);
    connect(quitAction, &QAction::triggered, this, &QCoreApplication::quit);

    configureHeightAction = new QAction("Set Height", this);
    connect(configureHeightAction, &QAction::triggered, this, &MainWindow::configureHeightClicked);
    configureHeightAction->setEnabled(false);
}

void MainWindow::configureHeightClicked()
{
    bool ok = false;

    auto height = QInputDialog::getInt(
        this,
        "Enter height",
        "Please measure the current height of the desk and enter the value in millimeters",
        bluetoothController->getCurrentHeightMm(),
        0,
        3000,
        1,
        &ok
    );

    if (!ok) {
        return;
    }

    auto heightMapping = Config::Model::HeightMapping{
        bluetoothController->getCurrentHeightRaw(),
        height
    };

    bluetoothController->setHeightMapping(heightMapping);
    config->setHeightMapping(heightMapping);
    saveConfig();
}
} // DeskControl::Gui::MainWindow
