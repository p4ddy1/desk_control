#include "DeskModel.h"

namespace DeskControl::Gui::Model
{
DeskModel::DeskModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int DeskModel::rowCount(const QModelIndex &parent) const
{
    return deskList.count();
}

QVariant DeskModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return {};
    }

    if (index.row() >= deskList.count() || index.row() < 0) {
        return {};
    }

    if (role == Qt::DisplayRole) {
        return {(deskList.at(index.row())->getName())};
    }

    return {};
}

void DeskModel::add(Desk *item)
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    deskList.append(item);
    endInsertRows();
}

void DeskModel::clear()
{
    beginResetModel();
    deskList.clear();
    endResetModel();
}

Desk *DeskModel::get(QModelIndex index) const
{
    return deskList.at(index.row());
}

Desk *DeskModel::get(int index) const
{
    return deskList.at(index);
}

void DeskModel::add(QList<Desk *> list)
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
    deskList.append(list);
    endInsertRows();
}

QList<Desk *> DeskModel::getDeskList() const
{
    return deskList;
}
} // Model