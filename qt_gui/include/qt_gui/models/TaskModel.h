/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

#include "qt_gui/models/AsyncListModel.h"
#include <core/CommandInvoker.h>
#include <core/ISprintStorageReader.h>
#include <core/ISprintStorageWriter.h>
#include <core/ITaskStorageReader.h>
#include <core/ITaskStorageWriter.h>
#include <core/QueryExecutor.h>
#include <core/entities/Task.h>
#include <date_wrapper/TimeSpan.h>

namespace sprint_timer::ui::qt_gui {

class TaskModel : public AsyncListModel {
    Q_OBJECT

public:
    TaskModel(ITaskStorageReader& taskReader,
              ITaskStorageWriter& taskWriter,
              ISprintStorageReader& sprintReader,
              ISprintStorageWriter& sprintWriter,
              CommandInvoker& commandInvoker,
              QueryExecutor& queryExecutor,
              QObject* parent = nullptr);

    // Override to support drag and drop.
    Qt::DropActions supportedDropActions() const override;

    // Override to support drag and drop.
    Qt::DropActions supportedDragActions() const override;

    // Override to support drag and drop.
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Override to provide support for custom roles as well as to customize
    // behaviour
    // of the default roles.
    QVariant data(const QModelIndex& index, int role) const override;

    // Override to support drag and drop. Changes items' priorities instead of
    // // removing row and inserting it at destination position as in default
    // behavour for drag and drop. That default behaviour would not work here,
    // as sqlite view is set to this model, and removing row from it would have
    // undesired consequences.
    bool moveRows(const QModelIndex& sourceParent,
                  int sourceRow,
                  int count,
                  const QModelIndex& destinationParent,
                  int destinationChild) override;

    int rowCount(const QModelIndex& parent) const override;

    enum customRoles {
        TagsRole = Qt::UserRole + 1,
        DescriptionRole,
        StatsRole,
        GetIdRole,
        GetFinishedSprintsRole,
        PriorityRole
    };

    // Insert new Task into the database.
    // Return boolean, indicating success of the operation.
    // Changes are rolled back in case of failure.
    void insert(const entities::Task& item);

    // Remove item with given index and return boolean, indicating success of
    // the operation.
    void remove(const QModelIndex& index);

    // Overload that accepts row number as item identifier.
    void remove(int row);

    // Return item at given row. This is a convinient method that allows to get
    // item
    // without verbose calls to data().
    entities::Task itemAt(int row) const;

    // Mark item as completed if it is not completed and vice versa.
    void toggleCompleted(const QModelIndex& index);

    // Replace data of item at given row with data from the newItem.
    void replaceItemAt(int row, const entities::Task& newItem);

protected:
    void requestDataUpdate() final;

private:
    ITaskStorageReader& taskReader;
    ITaskStorageWriter& taskWriter;
    ISprintStorageReader& sprintReader;
    ISprintStorageWriter& sprintWriter;
    CommandInvoker& commandInvoker;
    QueryExecutor& queryExecutor;
    std::vector<entities::Task> storage;
    // Sql helper queries that are needed to maintain database invariants.
    enum class Column {
        Id,
        Name,
        EstimatedCost,
        ActualCost,
        Priority,
        Completed,
        Tags,
        LastModified,
        Uuid
    };

    void onDataChanged(const std::vector<entities::Task>& tasks);

    void timerEvent(QTimerEvent* event) override;
};

} // namespace sprint_timer::ui::qt_gui


#endif /* end of include guard: TASKMODEL_H_FUQ5UCBE */
