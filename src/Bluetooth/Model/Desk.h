#ifndef DESK_CONTROL_DESK_H
#define DESK_CONTROL_DESK_H

#include <QString>
#include <QBluetoothUuid>
#include <QBluetoothAddress>
#include <QObject>

namespace DeskControl::Bluetooth::Model
{

class Desk: public QObject
{
Q_OBJECT
public:
    Desk(const QBluetoothUuid &uuid,
         const QBluetoothAddress &address,
         QString name,
         quint32 serviceClasses,
         QObject *parent = nullptr);

    QBluetoothUuid getUuid() const;

    QBluetoothAddress getAddress() const;

    QString getName() const;

    quint32 getServiceClasses() const;

private:
    QBluetoothUuid uuid;
    QBluetoothAddress address;
    QString name;
    quint32 serviceClasses;
};

} // Model

#endif //DESK_CONTROL_DESK_H
