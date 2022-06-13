#ifndef DESK_CONTROL_DEVICESCANWINDOW_H
#define DESK_CONTROL_DEVICESCANWINDOW_H

#include <QDialog>
#include "../../Bluetooth/Service/BluetoothScanner.h"
#include "../../Bluetooth/Model/Desk.h"
#include "../Model/DeskModel.h"

namespace DeskControl::Gui::DeviceScanWindow
{

using Bluetooth::Service::BluetoothScanner;
using Bluetooth::Model::Desk;
using Model::DeskModel;

QT_BEGIN_NAMESPACE
namespace Ui
{
class DeviceScanWindow;
}
QT_END_NAMESPACE

class DeviceScanWindow: public QDialog
{
Q_OBJECT

public:
    explicit DeviceScanWindow(QWidget *parent = nullptr);

    ~DeviceScanWindow() override;

private slots:

    void deskFound(Desk *desk);

    void startButtonClicked();

    void stopButtonClicked();

    void addButtonClicked();

    void scanFinished();

signals:

    void deviceConfigChanged(Desk *newDesk);

private:
    Ui::DeviceScanWindow *ui;
    BluetoothScanner *bluetoothScanner;
    DeskModel *deskModel;

    void setAddButtonState();
};
} // DeskControl::Gui::DeviceScanWindow

#endif //DESK_CONTROL_DEVICESCANWINDOW_H
