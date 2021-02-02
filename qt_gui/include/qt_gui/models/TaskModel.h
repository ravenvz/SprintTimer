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
#ifndef TASKMODEL_H_FUQ5UCBE
#define TASKMODEL_H_FUQ5UCBE

#include "qt_gui/presentation/TaskContract.h"
#include "qt_gui/presentation/TaskDTO.h"
#include <QAbstractListModel>

namespace sprint_timer::ui::qt_gui {

class TaskModel : public QAbstractListModel,
                  public contracts::TaskContract::View {

public:
    TaskModel(contracts::TaskContract::Presenter& presenter,
              QObject* parent = nullptr);

    ~TaskModel() override;

    void displayTasks(const std::vector<TaskDTO>& tasks) override;

    Qt::DropActions supportedDropActions() const override;

    Qt::DropActions supportedDragActions() const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    bool setData(const QModelIndex& index,
                 const QVariant& value,
                 int role = Qt::EditRole) override;

    bool moveRows(const QModelIndex& sourceParent,
                  int sourceRow,
                  int count,
                  const QModelIndex& destinationParent,
                  int destinationChild) override;

    bool removeRows(int row,
                    int count,
                    const QModelIndex& parent = QModelIndex{}) override;

    bool insertRows(int row,
                    int count,
                    const QModelIndex& parent = QModelIndex{}) override;

    int rowCount(const QModelIndex& parent) const override;

private:
    contracts::TaskContract::Presenter& presenter;
    std::vector<TaskDTO> storage;

    TaskDTO itemAt(int row) const;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: TASKMODEL_H_FUQ5UCBE */
