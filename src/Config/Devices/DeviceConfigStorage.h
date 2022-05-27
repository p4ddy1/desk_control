#ifndef DESK_CONTROL_DEVICECONFIGSTORAGE_H
#define DESK_CONTROL_DEVICECONFIGSTORAGE_H

#include <QList>
#include "../../Bluetooth/Model/Desk.h"

namespace DeskControl::Config::Devices {
            using Bluetooth::Model::Desk;

            class DeviceConfigStorage {
            public:
                bool save(QList<Desk*> deskList);
                QList<Desk*> load();
            };

        } // Devices

#endif //DESK_CONTROL_DEVICECONFIGSTORAGE_H
