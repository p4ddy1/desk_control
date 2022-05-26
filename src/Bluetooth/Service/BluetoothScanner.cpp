#include "BluetoothScanner.h"

namespace DeskControl::Bluetooth::Service {

    BluetoothScanner::BluetoothScanner(int discoveryTimeout, QObject* parent) : QObject(parent) {
        discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
        discoveryAgent->setLowEnergyDiscoveryTimeout(discoveryTimeout);

        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BluetoothScanner::deviceFound);
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BluetoothScanner::deviceScanFinished);
    }

    void BluetoothScanner::startScan() {
        deskList.clear();
        discoveryAgent->start();
    }

    void BluetoothScanner::deviceScanFinished() {
        emit scanFinished(deskList);
    }

    void BluetoothScanner::deviceFound(const QBluetoothDeviceInfo &info) {
        auto desk = new Desk(
                info.deviceUuid(),
                info.name(),
                info.serviceClasses()
                );

        deskList.append(desk);
    }

    void BluetoothScanner::deviceErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error) {
        emit errorOccurred("Some error"); //TODO: Implement this further
    }
} // Bluetooth