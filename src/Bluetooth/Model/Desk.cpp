#include "Desk.h"

#include <utility>

namespace DeskControl::Bluetooth::Model
{
Desk::Desk(const QBluetoothUuid &uuid, QString name, quint32 serviceClasses, QObject *parent)
    :
    uuid(uuid),
    name(std::move(name)),
    serviceClasses(serviceClasses),
    QObject(parent)
{
}

QBluetoothUuid Desk::getUuid() const
{
    return uuid;
}

QString Desk::getName() const
{
    return name;
}

quint32 Desk::getServiceClasses() const
{
    return serviceClasses;
}

} // Model