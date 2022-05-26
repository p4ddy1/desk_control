#ifndef DESK_CONTROL_DEVICESCANWINDOW_H
#define DESK_CONTROL_DEVICESCANWINDOW_H

#include <QDialog>

namespace DeskControl::Gui::DeviceScanWindow {
    QT_BEGIN_NAMESPACE
    namespace Ui { class DeviceScanWindow; }
    QT_END_NAMESPACE

    class DeviceScanWindow : public QDialog {
    Q_OBJECT

    public:
        explicit DeviceScanWindow(QWidget *parent = nullptr);

        ~DeviceScanWindow() override;

    private:
        Ui::DeviceScanWindow *ui;
    };
} // DeskControl::Gui::DeviceScanWindow

#endif //DESK_CONTROL_DEVICESCANWINDOW_H
