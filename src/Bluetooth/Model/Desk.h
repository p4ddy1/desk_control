#ifndef DESK_CONTROL_DESK_H
#define DESK_CONTROL_DESK_H

#include <QString>
#include <QBluetoothUuid>
#include <QObject>

namespace DeskControl::Bluetooth::Model {

            class Desk : public QObject {
                Q_OBJECT
            public:
                Desk(const QBluetoothUuid& uuid, QString name, quint32 serviceClasses, QObject* parent = nullptr);

                QBluetoothUuid getUuid() const;
                QString getName() const;
                quint32 getServiceClasses() const;
            private:
                QBluetoothUuid uuid;
                QString name;
                quint32 serviceClasses;
            };

        } // Model

#endif //DESK_CONTROL_DESK_H
