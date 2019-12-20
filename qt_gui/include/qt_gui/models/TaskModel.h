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

#include "qt_gui/DatasyncRelay.h"
#include "qt_gui/models/AsyncListModel.h"
#include <core/CommandInvoker.h>
#include <core/ISprintStorage.h>
#include <core/ITaskStorage.h>
#include <core/QueryInvoker.h>
#include <core/entities/Task.h>
#include <date_wrapper/date_wrapper.h>

namespace sprint_timer::ui::qt_gui {

class TaskModel : public AsyncListModel {
    Q_OBJECT

public:
    TaskModel(ITaskStorage& taskStorage,
              ISprintStorage& sprint,
              CommandInvoker& commandInvoker,
              QueryInvoker& queryInvoker,
              DatasyncRelay& datasyncRelay,
              QObject* parent = nullptr);

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
    ITaskStorage& taskStorage;
    ISprintStorage& sprintStorage;
    CommandInvoker& commandInvoker;
    QueryInvoker& queryInvoker;
    DatasyncRelay& datasyncRelay;
    std::vector<entities::Task> storage;

    void requestUpdate() final;

    void onDataChanged(const std::vector<entities::Task>& tasks);

    void insert(const entities::Task& item);

    void remove(const QModelIndex& index);

    void remove(int row);

    entities::Task itemAt(int row) const;

    void toggleCompleted(const QModelIndex& index);

    void replaceItemAt(int row, const entities::Task& newItem);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: TASKMODEL_H_FUQ5UCBE */
