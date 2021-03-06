#ifndef DESK_CONTROL_CONFIG_H
#define DESK_CONTROL_CONFIG_H

#include <QList>
#include "../../Bluetooth/Model/Desk.h"
#include "../../Gui/Model/Position.h"

namespace DeskControl::Config::Model
{
using Bluetooth::Model::Desk;
using Gui::Model::Position;

struct HeightMapping
{
    int heightRaw;
    int heightMm;
};

class Config
{
public:
    explicit Config(
        QList<Desk *> deskList = QList<Desk *>(),
        QList<Position *> positionList = QList<Position *>(),
        HeightMapping = HeightMapping{1760, 800}
    );

    QList<Desk *> getDeskList() const;

    void setDeskList(QList<Desk *> list);

    QList<Position *> getPositionList() const;

    void setPositionList(QList<Position *> list);

    HeightMapping getHeightMapping() const;

    void setHeightMapping(HeightMapping mapping);

private:
    QList<Desk *> deskList;
    QList<Position *> positionList;
    HeightMapping heightMapping;
};

} // Model

#endif //DESK_CONTROL_CONFIG_H
