#ifndef DESK_CONTROL_POSITION_H
#define DESK_CONTROL_POSITION_H

#include <QString>

namespace DeskControl::Gui::Model
{

class Position
{
public:
    Position(QString name, int heightMm);

    QString getName() const;

    int getHeightMm() const;

private:
    QString name;
    int heightMm;
};

} // Model

#endif //DESK_CONTROL_POSITION_H
