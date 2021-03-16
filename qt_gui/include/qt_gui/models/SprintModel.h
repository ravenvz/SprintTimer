/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef POMODOROMODEL_H_GKWGR80Y
#define POMODOROMODEL_H_GKWGR80Y

#include "qt_gui/presentation/TodaySprints.h"
#include <QAbstractListModel>
#include <core/entities/Sprint.h>
#include <vector>

namespace sprint_timer::ui::qt_gui {

class SprintModel : public QAbstractListModel,
                    public contracts::TodaySprints::View {

public:
    explicit SprintModel(contracts::TodaySprints::Presenter& presenter,
                         QObject* parent = nullptr);

    ~SprintModel() override;

    void displaySprints(const std::vector<SprintDTO>&) override;

    int rowCount(const QModelIndex& parent) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    bool removeRows(int row, int count, const QModelIndex& index) override;

    // bool
    // setData(const QModelIndex& index, const QVariant& value, int role)
    // override;

    // bool insertRows(int row, int count, const QModelIndex& parent) override;

private:
    contracts::TodaySprints::Presenter& presenter;
    std::vector<SprintDTO> storage;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: POMODOROMODEL_H_GKWGR80Y */
