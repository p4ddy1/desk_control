#include "BluetoothController.h"

namespace DeskControl::Bluetooth::Service
{
BluetoothController::BluetoothController(HeightMapping heightMapping, QObject *parent)
    :
    heightMapping(heightMapping), QObject(parent)
{

}

void BluetoothController::connectToDesk(Desk *deskToConnectTo)
{
    desk = deskToConnectTo;

    QBluetoothDeviceInfo deviceInfo;

    if (!desk->getAddress().isNull()) {
        deviceInfo = QBluetoothDeviceInfo(
            desk->getAddress(),
            desk->getName(),
            desk->getServiceClasses()
        );
    }
    else {
        deviceInfo = QBluetoothDeviceInfo(
            desk->getUuid(),
            desk->getName(),
            desk->getServiceClasses()
        );
    }

    controller = QLowEnergyController::createCentral(deviceInfo);

    connect(controller, &QLowEnergyController::connected, this, &BluetoothController::connectedToDevice);
    connect(controller, &QLowEnergyController::errorOccurred, this, &BluetoothController::errorOccurred);
    connect(controller, &QLowEnergyController::disconnected, this, &BluetoothController::disconnectedFromDevice);
    connect(controller, &QLowEnergyController::serviceDiscovered, this, &BluetoothController::serviceDiscovered);
    connect(controller, &QLowEnergyController::discoveryFinished, this,
            &BluetoothController::serviceDiscoveryFinished);

    controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
    controller->connectToDevice();
}

void BluetoothController::connectedToDevice()
{
    controller->discoverServices();
}

void BluetoothController::serviceDiscovered(const QBluetoothUuid &newService)
{
    if (newService == HEIGHT_SERVICE_UUID) {
        heightService = controller->createServiceObject(newService);

        connect(heightService, &QLowEnergyService::stateChanged, this,
                &BluetoothController::heightServiceDetailsDiscovered);
        connect(heightService, &QLowEnergyService::characteristicChanged, this,
                &BluetoothController::heightCharacteristicChanged);
        heightService->discoverDetails();
    }

    if (newService == MOVEMENT_SERVICE_UUID) {
        movementService = controller->createServiceObject(newService);
        movementService->discoverDetails();
    }
}

void BluetoothController::serviceDiscoveryFinished()
{
    if (heightService == nullptr) {
        emit connectionFailed("Missing Position Service");
        return;
    }

    if (movementService == nullptr) {
        emit connectionFailed("Missing Movement Service");
        return;
    }

    emit connected();
}

void BluetoothController::heightServiceDetailsDiscovered(QLowEnergyService::ServiceState newState)
{
    if (newState != QLowEnergyService::RemoteServiceDiscovered) {
        return;
    }

    bool updatesEnabled = false;

    for (const auto &characteristic : heightService->characteristics()) {
        auto uuid = characteristic.uuid();

        if (uuid == HEIGHT_CHARACTERISTIC_UUID) {
            for (const auto &descriptor : characteristic.descriptors()) {
                if (descriptor.name() == "Client Characteristic Configuration") {
                    heightService->writeDescriptor(descriptor, QByteArray::fromHex("0100")); //Enable Updates
                    updatesEnabled = true;
                }
            }

            int heightInMm = calculateHeightInMm(characteristic.value());
            emit heightChanged(heightInMm);
        }
    }

    if (!updatesEnabled) {
        emit connectionFailed("Could not enable updates");
    }
}

void BluetoothController::move(Direction direction)
{
    if (movementService == nullptr) {
        return;
    }

    QByteArray value;

    if (direction == Direction::Up) {
        value = QByteArray::fromHex("4700");
    }

    if (direction == Direction::Down) {
        value = QByteArray::fromHex("4600");
    }

    movementService->writeCharacteristic(
        movementService->characteristic(MOVEMENT_CHARACTERISTIC_UUID),
        value
    );
}

void BluetoothController::heightCharacteristicChanged(const QLowEnergyCharacteristic &info, const QByteArray &value)
{
    if (info.uuid() != HEIGHT_CHARACTERISTIC_UUID) {
        return;
    }

    int heightInMm = calculateHeightInMm(value);

    emit heightChanged(heightInMm);
}

int BluetoothController::calculateHeightInMm(const QByteArray &value)
{
    auto rawHeight = *(reinterpret_cast<const quint16_le *>(value.constData()));

    int heightMm = rawToMm(rawHeight);

    currentHeightMm = heightMm;
    currentHeightRaw = rawHeight;

    return heightMm;
}

int BluetoothController::rawToMm(int raw)
{
    return ((raw - heightMapping.heightRaw) / 10) + heightMapping.heightMm;
}

int BluetoothController::getCurrentHeightMm() const
{
    return currentHeightMm;
}

int BluetoothController::getCurrentHeightRaw() const
{
    return currentHeightRaw;
}

void BluetoothController::stop()
{
    movementService->writeCharacteristic(
        movementService->characteristic(MOVEMENT_CHARACTERISTIC_UUID),
        QByteArray::fromHex("FF")
    );
}

void BluetoothController::disconnectFromDesk()
{
    controller->disconnectFromDevice();
}

void BluetoothController::disconnectedFromDevice()
{
    delete heightService;
    delete movementService;
    emit disconnected();
}

void BluetoothController::errorOccurred(QLowEnergyController::Error newError)
{
    emit connectionFailed("Error while establishing connection");
}

void BluetoothController::setHeightMapping(HeightMapping mapping)
{
    heightMapping = mapping;
    emit heightChanged(rawToMm(currentHeightRaw));
}
} // Bluetooth