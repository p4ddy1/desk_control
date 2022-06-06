#ifndef DESK_CONTROL_CONFIGSTORAGE_H
#define DESK_CONTROL_CONFIGSTORAGE_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include "../Bluetooth/Model/Desk.h"
#include "../Gui/Model/Position.h"
#include "Model/Config.h"


namespace DeskControl::Config {
        using Bluetooth::Model::Desk;
        using Gui::Model::Position;
        using Model::Config;

        class ConfigStorage {
        public:
            explicit ConfigStorage(QString path);

            bool save(Config* config);
            Config* load();

        private:
            void convertDeskListToJson(QList<Desk*> list, QJsonObject& json);
            void convertPositionListToJson(QList<Position *> list, QJsonObject& json);

            QList<Desk *> convertJsonToDeskList(QJsonObject json);
            QList<Position *> convertJsonToPositionList(QJsonObject json);

            QString path;
        };

    } // Config

#endif //DESK_CONTROL_CONFIGSTORAGE_H
