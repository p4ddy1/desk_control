#ifndef DESK_CONTROL_MAINWINDOW_H
#define DESK_CONTROL_MAINWINDOW_H

#include <QWidget>
#include "../Model/DeskModel.h"
#include "../Model/PositionModel.h"
#include "../../Bluetooth/Service/BluetoothController.h"
#include "../../Bluetooth/Service/TargetHeightMovementService.h"
#include "../../Bluetooth/Model/Desk.h"
#include "../../Config/ConfigStorage.h"
#include "../../Config/Model/Config.h"

namespace DeskControl::Gui::MainWindow {
    using Model::DeskModel;
    using Model::PositionModel;
    using Bluetooth::Service::BluetoothController;
    using Bluetooth::Service::TargetHeightMovementService;
    using Bluetooth::Model::Desk;
    using Config::ConfigStorage;
    using Config::Model::Config;

    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE

    class MainWindow : public QWidget {
    Q_OBJECT

    public:
        MainWindow(ConfigStorage *configStorage, Config *config, QWidget *parent = nullptr);

        ~MainWindow() override;

    private slots:

        void connectButtonClicked();

        void disconnectButtonClicked();

        void connected();

        void disconnected();

        void connectionFailed(QString errorMessage);

        void upButtonClicked();

        void downButtonClicked();

        void heightChanged(int heightInMm);

        void scanButtonClicked();

        void deviceConfigChanged(Desk *newDesk);

        void addCurrentPositionButtonClicked();

        void deletePositionButtonClicked();

        void moveToPositionButtonClicked();

        void addPositionInputButtonClicked();

    private:
        Ui::MainWindow *ui;
        DeskModel *deskModel;
        PositionModel *positionModel;

        ConfigStorage *configStorage;
        Config *config;

        BluetoothController *bluetoothController;
        TargetHeightMovementService *targetHeightMovementService;

        int currentHeightMm;

        void showEvent(QShowEvent *event) override;

        void loadDeviceList();

        void loadPositionList();

        void savePositionList();

        QString askForPositionName();
    };
} // DeskControl::Gui::MainWindow

#endif //DESK_CONTROL_MAINWINDOW_H
