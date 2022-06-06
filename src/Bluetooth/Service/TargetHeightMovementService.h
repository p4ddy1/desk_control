#ifndef DESK_CONTROL_TARGETHEIGHTMOVEMENTSERVICE_H
#define DESK_CONTROL_TARGETHEIGHTMOVEMENTSERVICE_H

#include "BluetoothController.h"

namespace DeskControl::Bluetooth::Service {
    class TargetHeightMovementService : public QObject {
    Q_OBJECT

    public:
        TargetHeightMovementService(BluetoothController *bluetoothController, QObject *parent = nullptr);

        void moveToHeight(int heightMm);

    private slots:

        void heightChanged(int heightInMm);

        void movementTimerTimeout();

    private:
        BluetoothController *bluetoothController;
        Direction movementDirection;
        bool requestedMovement;
        int targetHeight;
        QTime lastMoveCommandSent;

        QTimer *movementTimer;

        void sendMovementCommand();

        void stopMovement();

        bool shouldMoveFurther();
    };

} // Service

#endif //DESK_CONTROL_TARGETHEIGHTMOVEMENTSERVICE_H
