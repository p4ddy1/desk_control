#include "Config.h"

namespace DeskControl::Config::Model
{
Config::Config(
    QList<Desk *> deskList,
    QList<Position *> positionList,
    HeightMapping heightMapping
)
    :
    deskList(std::move(deskList)),
    positionList(std::move(positionList)),
    heightMapping(heightMapping)
{
}

QList<Desk *> Config::getDeskList() const
{
    return deskList;
}

void Config::setDeskList(QList<Desk *> list)
{
    deskList = std::move(list);
}

QList<Position *> Config::getPositionList() const
{
    return positionList;
}

void Config::setPositionList(QList<Position *> list)
{
    positionList = std::move(list);
}

HeightMapping Config::getHeightMapping() const
{
    return heightMapping;
}

void Config::setHeightMapping(HeightMapping mapping)
{
    heightMapping = mapping;
}
} // Model