#include "TargetHeightMovementService.h"

namespace DeskControl::Bluetooth::Service {
    TargetHeightMovementService::TargetHeightMovementService(
            BluetoothController *bluetoothController,
            QObject *parent
    ) : bluetoothController(bluetoothController), QObject(parent) {
        movementTimer = new QTimer(this);

        connect(movementTimer, &QTimer::timeout, this, &TargetHeightMovementService::movementTimerTimeout);

        lastMoveCommandSent = QTime::currentTime();
        connect(bluetoothController, &BluetoothController::heightChanged, this,
                &TargetHeightMovementService::heightChanged);
    }

    void TargetHeightMovementService::moveToHeight(int heightMm) {
        if (bluetoothController->getCurrentHeightMm() == heightMm) {
            return;
        }

        targetHeight = heightMm;
        requestedMovement = true;

        if (bluetoothController->getCurrentHeightMm() > heightMm) {
            movementDirection = Direction::Down;
        } else {
            movementDirection = Direction::Up;
        }

        movementTimer->start(500);
    }

    void TargetHeightMovementService::heightChanged(int heightInMm) {
        if (!requestedMovement) {
            return;
        }

        if (shouldMoveFurther()) {
            return;
        }

        stopMovement();
    }

    void TargetHeightMovementService::sendMovementCommand() {
        bluetoothController->move(movementDirection);
    }

    void TargetHeightMovementService::movementTimerTimeout() {
        if (shouldMoveFurther()) {
            sendMovementCommand();
            return;
        }

        stopMovement();
    }

    bool TargetHeightMovementService::shouldMoveFurther() {
        auto heightInMm = bluetoothController->getCurrentHeightMm();


        if (movementDirection == Direction::Down && heightInMm > targetHeight) {
            return true;
        }

        if (movementDirection == Direction::Up && heightInMm < targetHeight) {
            return true;
        }

        return false;
    }

    void TargetHeightMovementService::stopMovement() {
        movementTimer->stop();
        bluetoothController->stop();
        requestedMovement = false;
    }
} // Service