#include "BluetoothScanner.h"

namespace DeskControl::Bluetooth::Service
{

BluetoothScanner::BluetoothScanner(int discoveryTimeout, QObject *parent)
    : QObject(parent)
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    discoveryAgent->setLowEnergyDiscoveryTimeout(discoveryTimeout);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this,
            &BluetoothScanner::deviceFound);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BluetoothScanner::deviceScanFinished);
}

void BluetoothScanner::startScan()
{
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void BluetoothScanner::deviceScanFinished()
{
    emit scanFinished();
}

void BluetoothScanner::deviceFound(const QBluetoothDeviceInfo &info)
{
    auto desk = new Desk(
        info.deviceUuid(),
        info.name(),
        info.serviceClasses()
    );

    emit deskFound(desk);
}

void BluetoothScanner::deviceErrorOccurred(QBluetoothDeviceDiscoveryAgent::Error error)
{
    emit errorOccurred("Some error"); //TODO: Implement this further
}

void BluetoothScanner::stopScan()
{
    discoveryAgent->stop();
}
} // Bluetooth