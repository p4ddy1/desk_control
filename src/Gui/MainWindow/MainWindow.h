#ifndef DESK_CONTROL_MAINWINDOW_H
#define DESK_CONTROL_MAINWINDOW_H

#include <QWidget>
#include "../Model/DeskModel.h"
#include "../../Bluetooth/Service/BluetoothController.h"
#include "../../Config/Devices/DeviceConfigStorage.h"

namespace DeskControl::Gui::MainWindow {
    using Model::DeskModel;
    using Bluetooth::Service::BluetoothController;
    using Config::Devices::DeviceConfigStorage;

    QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
    QT_END_NAMESPACE

    class MainWindow : public QWidget {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow() override;

    private slots:
        void connectButtonClicked();
        void connected();
        void connectionFailed(QString errorMessage);
        void upButtonClicked();
        void downButtonClicked();
        void heightChanged(int heightInMm);
        void scanButtonClicked();
        void deviceConfigChanged();

    private:
        Ui::MainWindow *ui;
        DeskModel* deskModel;

        BluetoothController* bluetoothController;
        DeviceConfigStorage* deviceConfigStorage;

        void showEvent(QShowEvent *event) override;
        void loadDeviceList();
    };
} // DeskControl::Gui::MainWindow

#endif //DESK_CONTROL_MAINWINDOW_H
