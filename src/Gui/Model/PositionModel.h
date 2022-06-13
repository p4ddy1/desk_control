#ifndef DESK_CONTROL_POSITIONMODEL_H
#define DESK_CONTROL_POSITIONMODEL_H

#include <QAbstractTableModel>
#include "Position.h"

namespace DeskControl::Gui::Model
{

class PositionModel: public QAbstractTableModel
{
Q_OBJECT

public:
    explicit PositionModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;

    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void add(Position *item);

    void add(QList<Position *> list);

    void clear();

    Position *get(QModelIndex index);

    void remove(QModelIndex index);

    QList<Position *> getPositionList() const;

private:
    QList<Position *> positionList;
};

} // Model

#endif //DESK_CONTROL_POSITIONMODEL_H
