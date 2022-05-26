#ifndef DESK_CONTROL_BLUETOOTHCONTROLLER_H
#define DESK_CONTROL_BLUETOOTHCONTROLLER_H

#include <QtBluetooth>
#include "../Model/Desk.h"

namespace DeskControl::Bluetooth::Service {
    using Model::Desk;

        struct HeightMapping {
            int heightRaw;
            int heightMm;
        };

        enum Direction {
            Up,
            Down
        };

        class BluetoothController : public QObject {
            Q_OBJECT
        public:
            explicit BluetoothController(HeightMapping* heightMapping, QObject* parent = nullptr);

            void connectToDesk(Desk* deskToConnectTo);
            void move(Direction direction);

        private slots:
            void connectedToDevice();
            void serviceDiscovered(const QBluetoothUuid &newService);
            void serviceDiscoveryFinished();
            void heightServiceDetailsDiscovered(QLowEnergyService::ServiceState newState);
            void heightCharacteristicChanged(const QLowEnergyCharacteristic &info,
                                       const QByteArray &value);

        signals:
            void connectionFailed(QString errorMessage);
            void connected();
            void heightChanged(int heightInMm);

        private:
            Desk* desk;
            QLowEnergyController* controller;
            QLowEnergyService* heightService;
            QLowEnergyService* movementService;
            HeightMapping* heightMapping;

            const QBluetoothUuid HEIGHT_SERVICE_UUID = QBluetoothUuid("{99fa0020-338a-1024-8a49-009c0215f78a}");
            const QBluetoothUuid HEIGHT_CHARACTERISTIC_UUID = QBluetoothUuid("{99fa0021-338a-1024-8a49-009c0215f78a}");
            const QBluetoothUuid MOVEMENT_SERVICE_UUID = QBluetoothUuid("{99fa0001-338a-1024-8a49-009c0215f78a}");
            const QBluetoothUuid MOVEMENT_CHARACTERISTIC_UUID = QBluetoothUuid("{99fa0002-338a-1024-8a49-009c0215f78a}");

            int calculateHeightInMm(const QByteArray &value);
        };

    } // Bluetooth

#endif //DESK_CONTROL_BLUETOOTHCONTROLLER_H