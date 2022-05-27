#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include "DeviceConfigStorage.h"

namespace DeskControl::Config::Devices {
    bool DeviceConfigStorage::save(QList<Desk *> deskList) {
        QJsonObject deviceList;
        QJsonArray deskArray;

        for (auto desk : deskList) {
            QJsonObject jsonDesk;
            jsonDesk["uuid"] = desk->getUuid().toString();
            jsonDesk["name"] = desk->getName();
            jsonDesk["serviceClasses"] = (int)desk->getServiceClasses();

            deskArray.append(jsonDesk);
        }

        deviceList["deskList"] = deskArray;

        QFile saveFile("devices.json");

        if (!saveFile.open(QIODevice::WriteOnly)) {
            return false;
        }

        QJsonDocument saveDocument(deviceList);
        saveFile.write(saveDocument.toJson());

        return true;
    }

    QList<Desk *> DeviceConfigStorage::load() {
        QFile loadFile("devices.json");

        if (!loadFile.open(QIODevice::ReadOnly)) {
            return {};
        }

        QList<Desk*> deskList;

        auto deviceData = loadFile.readAll();

        auto deskListObject = QJsonDocument::fromJson(deviceData).object();

        auto deskArray = deskListObject["deskList"].toArray();

        for(auto deskEntry : deskArray) {
            auto deskObject = deskEntry.toObject();

            deskList.append(new Desk(
                    QBluetoothUuid(deskObject["uuid"].toString()),
                    deskObject["name"].toString(),
                    (quint32)deskObject["serviceClasses"].toInteger()
                ));
        }

        return deskList;
    }
} // Devices