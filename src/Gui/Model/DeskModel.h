#ifndef DESK_CONTROL_DESKMODEL_H
#define DESK_CONTROL_DESKMODEL_H

#include <QAbstractListModel>
#include "../../Bluetooth/Model/Desk.h"

namespace DeskControl::Gui::Model {
    using Bluetooth::Model::Desk;

            class DeskModel : public QAbstractListModel {
                Q_OBJECT
            public:
                explicit DeskModel(QObject* parent = nullptr);

                int rowCount(const QModelIndex &parent) const override;
                QVariant data(const QModelIndex &index, int role) const override;

                void add(Desk* item);
                void add(QList<Desk*> list);
                void clear();
                Desk* get(QModelIndex index) const;
                Desk* get(int index) const;

            private:
                QList<Desk*> deskList;
            };

        } // Model

#endif //DESK_CONTROL_DESKMODEL_H
