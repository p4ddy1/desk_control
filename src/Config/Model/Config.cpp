#include "Config.h"

namespace DeskControl::Config::Model {
    Config::Config(QList<Desk *> deskList, QList<Position *> positionList) :
            deskList(std::move(deskList)), positionList(std::move(positionList)) {

    }

    QList<Desk *> Config::getDeskList() const {
        return deskList;
    }

    void Config::setDeskList(QList<Desk *> list) {
        deskList = std::move(list);
    }

    QList<Position *> Config::getPositionList() const {
        return positionList;
    }

    void Config::setPositionList(QList<Position *> list) {
        positionList = std::move(list);
    }
} // Model