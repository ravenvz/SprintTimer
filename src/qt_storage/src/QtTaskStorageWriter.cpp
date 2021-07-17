/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include "qt_storage/QtTaskStorageWriter.h"
#include "qt_storage/DatabaseDescription.h"
#include "qt_storage/TransactionGuard.h"
#include "qt_storage/utils.h"
#include "qt_storage/utils/DateTimeConverter.h"
#include "qt_storage/utils/QueryUtils.h"
#include <QVariant>

namespace sprint_timer::storage::qt_storage {

using namespace qt_storage;

QtTaskStorageWriter::QtTaskStorageWriter(QString connectionName_)
    : connectionName{std::move(connectionName_)}
{
    createTaskQuery =
        tryPrepare(connectionName,
                   QString{"INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8) "
                           "VALUES (:name, :estimated_cost, :actual_cost, "
                           ":completed, "
                           ":priority, :last_modified, :uuid); "}
                       .arg(TaskTable::name)
                       .arg(TaskTable::Columns::name)
                       .arg(TaskTable::Columns::estimatedCost)
                       .arg(TaskTable::Columns::actualCost)
                       .arg(TaskTable::Columns::completed)
                       .arg(TaskTable::Columns::priority)
                       .arg(TaskTable::Columns::lastModified)
                       .arg(TaskTable::Columns::uuid));
    createTagQuery =
        tryPrepare(connectionName,
                   QString{"INSERT INTO %1(%2, %3) VALUES(:tag, :uuid);"}
                       .arg(TaskTagView::name)
                       .arg(TaskTagView::Aliases::tagName)
                       .arg(TaskTable::Columns::uuid));
    deleteTagQuery =
        tryPrepare(connectionName,
                   QString{"DELETE FROM %1 WHERE %2 = :uuid "
                           "AND %3 IN (SELECT ID FROM %4 WHERE %5 = :tag);"}
                       .arg(TaskTagTable::name)
                       .arg(TaskTagTable::Columns::taskUuid)
                       .arg(TaskTagTable::Columns::tagId)
                       .arg(TagTable::name)
                       .arg(TagTable::Columns::name));
    deleteTaskQuery = tryPrepare(connectionName,
                                 QString{"DELETE FROM %1 WHERE %2 = :uuid;"}
                                     .arg(TaskTable::name)
                                     .arg(TaskTable::Columns::uuid));
    editTaskQuery = tryPrepare(connectionName,
                               QString{"UPDATE %1 SET %2 = :name, "
                                       "%3 = :estimated_cost, "
                                       "%4 = :last_modified "
                                       "WHERE %5 = :uuid;"}
                                   .arg(TaskTable::name)
                                   .arg(TaskTable::Columns::name)
                                   .arg(TaskTable::Columns::estimatedCost)
                                   .arg(TaskTable::Columns::lastModified)
                                   .arg(TaskTable::Columns::uuid));
    toggleCompletionQuery =
        tryPrepare(connectionName,
                   QString{"UPDATE %1 "
                           "SET %2 = not %2, "
                           "%3 = :time_stamp "
                           "WHERE %4 = :uuid;"}
                       .arg(TaskTable::name)
                       .arg(TaskTable::Columns::completed)
                       .arg(TaskTable::Columns::lastModified)
                       .arg(TaskTable::Columns::uuid));
    updatePrioritiesQuery = tryPrepare(connectionName,
                                       QString{"UPDATE %1 SET %2 = :priority "
                                               "WHERE %3 = :uuid;"}
                                           .arg(TaskTable::name)
                                           .arg(TaskTable::Columns::priority)
                                           .arg(TaskTable::Columns::uuid));
    editTagQuery = tryPrepare(connectionName,
                              QString{"UPDATE %1 SET %2 = :new_name WHERE %2 = "
                                      ":old_name;"}
                                  .arg(TagTable::name)
                                  .arg(TagTable::Columns::name));
}

void QtTaskStorageWriter::save(const entities::Task& task)
{
    const QString uuid = QString::fromStdString(task.uuid());
    createTaskQuery.bindValue(":name", QString::fromStdString(task.name()));
    createTaskQuery.bindValue(":estimated_cost", task.estimatedCost());
    createTaskQuery.bindValue(":actual_cost", task.actualCost());
    createTaskQuery.bindValue(":completed", task.isCompleted());
    createTaskQuery.bindValue(":priority", 10000);
    createTaskQuery.bindValue(
        ":last_modified",
        utils::DateTimeConverter::qDateTime(task.lastModified()));
    createTaskQuery.bindValue(":uuid", uuid);

    TransactionGuard guard{connectionName};
    tryExecute(createTaskQuery);
    insertTags(uuid, task.tags());
    guard.commit();
}

void QtTaskStorageWriter::remove(const std::string& uuid)
{
    // Note that tags are removed by sqlite trigger
    deleteTaskQuery.bindValue(":uuid", QString::fromStdString(uuid));
    tryExecute(deleteTaskQuery);
}

void QtTaskStorageWriter::edit(const entities::Task& oldTask,
                               const entities::Task& editedTask)
{
    using namespace utils;

    const QString taskUuid = QString::fromStdString(oldTask.uuid());

    editTaskQuery.bindValue(":name", QString::fromStdString(editedTask.name()));
    editTaskQuery.bindValue(":estimated_cost", editedTask.estimatedCost());
    editTaskQuery.bindValue(
        ":last_modified",
        DateTimeConverter::qDateTime(editedTask.lastModified()));
    editTaskQuery.bindValue(":uuid", taskUuid);

    auto oldTags = oldTask.tags();
    auto newTags = editedTask.tags();
    oldTags.sort();
    newTags.sort();
    std::list<entities::Tag> tagsToRemove;
    std::list<entities::Tag> tagsToInsert;

    twoWayDiff(cbegin(oldTags),
               cend(oldTags),
               cbegin(newTags),
               cend(newTags),
               std::back_inserter(tagsToRemove),
               std::back_inserter(tagsToInsert));

    TransactionGuard guard{connectionName};
    tryExecute(editTaskQuery);
    removeTags(taskUuid, tagsToRemove);
    insertTags(taskUuid, tagsToInsert);
    guard.commit();
}

void QtTaskStorageWriter::toggleCompleted(const std::string& uuid,
                                          const dw::DateTime& timeStamp)
{
    toggleCompletionQuery.bindValue(":uuid", QString::fromStdString(uuid));
    toggleCompletionQuery.bindValue(
        ":time_stamp", utils::DateTimeConverter::qDateTime(timeStamp));
    tryExecute(toggleCompletionQuery);
}

void QtTaskStorageWriter::updatePriorities(
    const std::vector<std::string>& priorities)
{
    TransactionGuard guard{connectionName};

    for (size_t i = 0; i < priorities.size(); ++i) {
        updatePrioritiesQuery.bindValue(":uuid",
                                        QString::fromStdString(priorities[i]));
        updatePrioritiesQuery.bindValue(":priority", static_cast<int>(i));
        tryExecute(updatePrioritiesQuery);
    }

    guard.commit();
}

void QtTaskStorageWriter::editTag(const std::string& oldName,
                                  const std::string& newName)
{
    editTagQuery.bindValue(":old_name", QString::fromStdString(oldName));
    editTagQuery.bindValue(":new_name", QString::fromStdString(newName));
    tryExecute(editTagQuery);
}

void QtTaskStorageWriter::insertTags(const QString& taskUuid,
                                     const std::list<entities::Tag>& tags)
{
    for (const auto& tag : tags) {
        createTagQuery.bindValue(":tag", QString::fromStdString(tag.name()));
        createTagQuery.bindValue(":uuid", taskUuid);
        tryExecute(createTagQuery);
    }
}

void QtTaskStorageWriter::removeTags(const QString& taskUuid,
                                     const std::list<entities::Tag>& tags)
{
    for (const auto& tag : tags) {
        deleteTagQuery.bindValue(":uuid", taskUuid);
        deleteTagQuery.bindValue(":tag", QString::fromStdString(tag.name()));
        tryExecute(deleteTagQuery);
    }
}

} // namespace sprint_timer::storage::qt_storage
