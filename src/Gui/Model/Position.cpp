#include "Position.h"

#include <utility>

namespace DeskControl::Gui::Model {
    Position::Position(QString name, int heightMm) : name(std::move(name)), heightMm(heightMm) {
    }

    QString Position::getName() const {
        return name;
    }

    int Position::getHeightMm() const {
        return heightMm;
    }
} // Model