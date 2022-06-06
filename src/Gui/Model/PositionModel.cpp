#include "PositionModel.h"

namespace DeskControl::Gui::Model {
    PositionModel::PositionModel(QObject *parent) : QAbstractTableModel(parent) {

    }

    int PositionModel::rowCount(const QModelIndex &parent) const {
        return positionList.count();
    }

    QVariant PositionModel::data(const QModelIndex &index, int role) const {
        if (!index.isValid()) {
            return {};
        }

        if (index.row() >= positionList.count() || index.row() < 0) {
            return {};
        }

        if (role != Qt::DisplayRole) {
            return {};
        }

        auto height = positionList.at(index.row());

        if (index.column() == 0) {
            return {height->getName()};
        }

        if (index.column() == 1) {
            return {height->getHeightMm() / 10};
        }

        return {};
    }

    QVariant PositionModel::headerData(int section, Qt::Orientation orientation, int role) const {
        if (orientation != Qt::Orientation::Horizontal) {
            return {};
        }

        if (role != Qt::DisplayRole) {
            return {};
        }

        if (section == 0) {
            return {"Name"};
        }

        if (section == 1) {
            return {"cm"};
        }

        return {};
    }

    int PositionModel::columnCount(const QModelIndex &parent) const {
        return 2;
    }

    void PositionModel::add(Position *item) {
        beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));
        positionList.append(item);
        endInsertRows();
    }

    void PositionModel::add(QList<Position *> list) {
        beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()) + list.size());
        positionList.append(list);
        endInsertRows();
    }

    void PositionModel::clear() {
        beginRemoveRows(QModelIndex(), 0, rowCount(QModelIndex()));
        positionList.clear();
        endRemoveRows();
    }

    QList<Position *> PositionModel::getPositionList() const {
        return positionList;
    }

    Position *PositionModel::get(QModelIndex index) {
        return positionList.at(index.row());
    }

    void PositionModel::remove(QModelIndex index) {
        if (!index.isValid()) {
            return;
        }

        beginRemoveRows(QModelIndex(), index.row(), index.row());
        positionList.remove(index.row());
        endRemoveRows();
    }
} // Model