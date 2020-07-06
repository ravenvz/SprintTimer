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
#ifndef REPLACEMODEL_H_CC0DKQIN
#define REPLACEMODEL_H_CC0DKQIN

#include "qt_gui/DatasyncRelay.h"
#include <core/CommandHandler.h>
#include <core/QueryHandler.h>
#include <core/SprintStorage.h>
#include <core/use_cases/change_tasks_priority/ChangeUnfinishedTasksPriorityCommand.h>
#include <core/use_cases/create_task/CreateTaskCommand.h>
#include <core/use_cases/delete_task/DeleteTaskCommand.h>
#include <core/use_cases/edit_task/EditTaskCommand.h>
#include <core/use_cases/request_tasks/UnfinishedTasksQuery.h>
#include <core/use_cases/toggle_task_completed/ToggleTaskCompletedCommand.h>
#include <date_wrapper/date_wrapper.h>

namespace sprint_timer::ui::qt_gui {

class ReplaceModel : public QAbstractListModel {

public:
    ReplaceModel(QObject* parent = nullptr);

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
    std::vector<entities::Task> storage;

    void onDataChanged(const std::vector<entities::Task>& tasks);

    void insert(const entities::Task& item);

    void remove(const QModelIndex& index);

    void remove(int row);

    entities::Task itemAt(int row) const;

    void toggleCompleted(const QModelIndex& index);

    void replaceItemAt(int row, entities::Task&& newItem);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: REPLACEMODEL_H_CC0DKQIN */
