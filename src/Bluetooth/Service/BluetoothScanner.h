#ifndef DESK_CONTROL_BLUETOOTHSCANNER_H
#define DESK_CONTROL_BLUETOOTHSCANNER_H

#include <QtBluetooth>
#include "../Model/Desk.h"

namespace DeskControl::Bluetooth::Service {
    using Model::Desk;

        class BluetoothScanner : public QObject {
            Q_OBJECT
        public:
            explicit BluetoothScanner(int discoveryTimeout = 20000, QObject* parent = nullptr);

            void startScan();
        private:
        QBluetoothDeviceDiscoveryAgent* discoveryAgent;
        QList<Desk*> deskList;

        private slots:
            void deviceScanFinished();
            void deviceFound(const QBluetoothDeviceInfo &info);
            void deviceErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error);

        signals:
            void scanFinished(const QList<Desk*> foundDeskList);
            void errorOccurred(QString error);
        };

    } // Bluetooth

#endif //DESK_CONTROL_BLUETOOTHSCANNER_H
