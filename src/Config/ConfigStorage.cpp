#include <QStandardPaths>
#include <QDir>
#include "ConfigStorage.h"

namespace DeskControl::Config
{
ConfigStorage::ConfigStorage(QString path)
    : path(std::move(path))
{
}

bool ConfigStorage::save(Config *config)
{
    QFile saveFile(getAbsolutePath());

    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }

    auto json = new QJsonObject();

    convertDeskListToJson(config->getDeskList(), *json);
    convertPositionListToJson(config->getPositionList(), *json);
    convertHeightMappingToJson(config->getHeightMapping(), *json);

    QJsonDocument document(*json);
    saveFile.write(document.toJson());

    return true;
}

Config *ConfigStorage::load()
{
    QFile loadFile(getAbsolutePath());

    if (!loadFile.open(QIODevice::ReadOnly)) {
        return new Config();
    }

    auto data = loadFile.readAll();

    auto json = QJsonDocument::fromJson(data).object();

    auto deskList = convertJsonToDeskList(json);
    auto positionList = convertJsonToPositionList(json);
    auto heightMapping = convertJsonToHeightMapping(json);

    return new Config(deskList, positionList, heightMapping);
}

void ConfigStorage::convertDeskListToJson(QList<Desk *> list, QJsonObject &json)
{
    QJsonArray deskArray;

    for (auto desk : list) {
        QJsonObject jsonDesk;
        jsonDesk["uuid"] = desk->getUuid().toString();
        jsonDesk["address"] = desk->getAddress().toString();
        jsonDesk["name"] = desk->getName();
        jsonDesk["serviceClasses"] = (int) desk->getServiceClasses();

        deskArray.append(jsonDesk);
    }

    json["deskList"] = deskArray;
}

void ConfigStorage::convertPositionListToJson(QList<Position *> list, QJsonObject &json)
{
    QJsonArray positionArray;

    for (auto position : list) {
        QJsonObject jsonPosition;
        jsonPosition["name"] = position->getName();
        jsonPosition["heightMm"] = position->getHeightMm();

        positionArray.append(jsonPosition);
    }

    json["positionList"] = positionArray;
}

QList<Desk *> ConfigStorage::convertJsonToDeskList(QJsonObject json)
{
    QList<Desk *> deskList;

    auto deskArray = json["deskList"].toArray();

    for (auto deskEntry : deskArray) {
        auto deskObject = deskEntry.toObject();

        deskList.append(new Desk(
            QBluetoothUuid(deskObject["uuid"].toString()),
            QBluetoothAddress(deskObject["address"].toString()),
            deskObject["name"].toString(),
            (quint32) deskObject["serviceClasses"].toInteger()
        ));
    }

    return deskList;
}

QList<Position *> ConfigStorage::convertJsonToPositionList(QJsonObject json)
{
    QList<Position *> positionList;

    auto positionArray = json["positionList"].toArray();

    for (auto positionEntry : positionArray) {
        auto positionObject = positionEntry.toObject();

        positionList.append(new Position(
            positionObject["name"].toString(),
            positionObject["heightMm"].toInt()
        ));
    }

    return positionList;
}

QString ConfigStorage::getAbsolutePath()
{
    auto configBasePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    if (configBasePath.isEmpty()) {
        qFatal("Error finding config path");
    }

    QDir configDir(configBasePath);

    if (!configDir.exists()) {
        configDir.mkpath(configBasePath);
    }

    return configDir.absoluteFilePath(path);
}

void ConfigStorage::convertHeightMappingToJson(Model::HeightMapping heightMapping, QJsonObject &json)
{
    QJsonObject jsonHeightMapping;

    jsonHeightMapping["heightRaw"] = heightMapping.heightRaw;
    jsonHeightMapping["heightMm"] = heightMapping.heightMm;

    json["heightMapping"] = jsonHeightMapping;
}

Model::HeightMapping ConfigStorage::convertJsonToHeightMapping(QJsonObject json)
{
    auto jsonHeightMapping = json["heightMapping"].toObject();

    return Model::HeightMapping{
        jsonHeightMapping["heightRaw"].toInt(),
        jsonHeightMapping["heightMm"].toInt(),
    };
}
} // Config