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
#include "qt_storage_impl/QtTaskStorageWriter.h"
#include "qt_storage_impl/DatabaseDescription.h"
#include "qt_storage_impl/utils.h"
#include "qt_storage_impl/utils/DateTimeConverter.h"

namespace sprint_timer::storage::qt_storage_impl {

using namespace entities;
using namespace dw;

QtTaskStorageWriter::QtTaskStorageWriter(DBService& dbService)
    : dbService{dbService}
{
    addTaskQueryId = dbService.prepare(
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
    insertTagQueryId = dbService.prepare(
        QString{"INSERT INTO %1(%2, %3) VALUES(:tag, :uuid);"}
            .arg(TaskTagView::name)
            .arg(TaskTagView::Aliases::tagName)
            .arg(TaskTable::Columns::uuid));
    removeTagQueryId = dbService.prepare(
        QString{"DELETE FROM %1 WHERE %2 = :uuid "
                "AND %3 IN (SELECT ID FROM %4 WHERE %5 = :tag);"}
            .arg(TaskTagTable::name)
            .arg(TaskTagTable::Columns::taskUuid)
            .arg(TaskTagTable::Columns::tagId)
            .arg(TagTable::name)
            .arg(TagTable::Columns::name));
    removeTaskQueryId
        = dbService.prepare(QString{"DELETE FROM %1 WHERE %2 = :uuid;"}
                                .arg(TaskTable::name)
                                .arg(TaskTable::Columns::uuid));
    editQueryId = dbService.prepare(QString{"UPDATE %1 SET %2 = :name, "
                                            "%3 = :estimated_cost, "
                                            "%4 = :last_modified "
                                            "WHERE %5 = :uuid;"}
                                        .arg(TaskTable::name)
                                        .arg(TaskTable::Columns::name)
                                        .arg(TaskTable::Columns::estimatedCost)
                                        .arg(TaskTable::Columns::lastModified)
                                        .arg(TaskTable::Columns::uuid));
    incrementSpentQueryId
        = dbService.prepare(QString{"UPDATE %1 set %2 = %2 + 1 "
                                    "WHERE %3.%4 = (:todo_uuid);"}
                                .arg(TaskTable::name)
                                .arg(TaskTable::Columns::actualCost)
                                .arg(TaskTable::name)
                                .arg(TaskTable::Columns::uuid));
    decrementSpentQueryId
        = dbService.prepare(QString{"UPDATE %1 SET %2 = %2 - 1 "
                                    "WHERE %3.%4 = (:todo_uuid);"}
                                .arg(TaskTable::name)
                                .arg(TaskTable::Columns::actualCost)
                                .arg(TaskTable::name)
                                .arg(TaskTable::Columns::uuid));
    toggleCompletionQueryId
        = dbService.prepare(QString{"UPDATE %1 "
                                    "SET %2 = not %2, "
                                    "%3 = :time_stamp "
                                    "WHERE %4 = :uuid;"}
                                .arg(TaskTable::name)
                                .arg(TaskTable::Columns::completed)
                                .arg(TaskTable::Columns::lastModified)
                                .arg(TaskTable::Columns::uuid));
    updatePrioritiesQueryId
        = dbService.prepare(QString{"UPDATE %1 SET %2 = :priority "
                                    "WHERE %3 = :uuid;"}
                                .arg(TaskTable::name)
                                .arg(TaskTable::Columns::priority)
                                .arg(TaskTable::Columns::uuid));
    editTagQueryId
        = dbService.prepare(QString{"UPDATE %1 SET %2 = :new_name WHERE %2 = "
                                    ":old_name;"}
                                .arg(TagTable::name)
                                .arg(TagTable::Columns::name));
}

void QtTaskStorageWriter::save(const Task& task)
{
    QString uuid = QString::fromStdString(task.uuid());
    dbService.bindValue(
        addTaskQueryId, ":name", QString::fromStdString(task.name()));
    dbService.bindValue(
        addTaskQueryId, ":estimated_cost", task.estimatedCost());
    dbService.bindValue(addTaskQueryId, ":actual_cost", task.actualCost());
    dbService.bindValue(addTaskQueryId, ":completed", task.isCompleted());
    dbService.bindValue(addTaskQueryId, ":priority", 10000);

    dbService.bindValue(
        addTaskQueryId,
        ":last_modified",
        utils::DateTimeConverter::qDateTime(task.lastModified()));
    dbService.bindValue(addTaskQueryId, ":uuid", uuid);

    dbService.transaction();

    dbService.executePrepared(addTaskQueryId);
    insertTags(uuid, task.tags());

    dbService.commit();
}

void QtTaskStorageWriter::insertTags(const QString& taskUuid,
                                     const std::list<Tag>& tags)
{
    for (const auto& tag : tags) {
        dbService.bindValue(
            insertTagQueryId, ":tag", QString::fromStdString(tag.name()));
        dbService.bindValue(insertTagQueryId, ":uuid", taskUuid);
        dbService.executePrepared(insertTagQueryId);
    }
}

void QtTaskStorageWriter::removeTags(const QString& taskUuid,
                                     const std::list<Tag>& tags)
{
    for (const auto& tag : tags) {
        dbService.bindValue(removeTagQueryId, ":uuid", taskUuid);
        dbService.bindValue(
            removeTagQueryId, ":tag", QString::fromStdString(tag.name()));
        dbService.executePrepared(removeTagQueryId);
    }
}

void QtTaskStorageWriter::remove(const Task& task)
{
    // Note that tags are removed by trigger
    dbService.bindValue(
        removeTaskQueryId, ":uuid", QString::fromStdString(task.uuid()));
    dbService.executePrepared(removeTaskQueryId);
}

void QtTaskStorageWriter::edit(const Task& task, const Task& editedTask)
{
    using namespace utils;

    const QString taskUuid = QString::fromStdString(task.uuid());

    dbService.bindValue(
        editQueryId, ":name", QString::fromStdString(editedTask.name()));
    dbService.bindValue(
        editQueryId, ":estimated_cost", editedTask.estimatedCost());
    dbService.bindValue(
        editQueryId,
        ":last_modified",
        DateTimeConverter::qDateTime(editedTask.lastModified()));
    dbService.bindValue(editQueryId, ":uuid", taskUuid);

    auto oldTags = task.tags();
    auto newTags = editedTask.tags();
    oldTags.sort();
    newTags.sort();
    std::list<Tag> tagsToRemove;
    std::list<Tag> tagsToInsert;

    twoWayDiff(cbegin(oldTags),
               cend(oldTags),
               cbegin(newTags),
               cend(newTags),
               std::back_inserter(tagsToRemove),
               std::back_inserter(tagsToInsert));

    dbService.transaction();

    dbService.executePrepared(editQueryId);
    removeTags(taskUuid, tagsToRemove);
    insertTags(taskUuid, tagsToInsert);

    dbService.commit();
}

void QtTaskStorageWriter::incrementSprints(const std::string& uuid)
{
    dbService.bindValue(
        incrementSpentQueryId, ":todo_uuid", QString::fromStdString(uuid));
    dbService.executePrepared(incrementSpentQueryId);
}

void QtTaskStorageWriter::decrementSprints(const std::string& uuid)
{
    dbService.bindValue(
        decrementSpentQueryId, ":todo_uuid", QString::fromStdString(uuid));
    dbService.executePrepared(decrementSpentQueryId);
}

void QtTaskStorageWriter::toggleTaskCompletionStatus(const std::string& uuid,
                                                     const DateTime& timeStamp)
{
    dbService.bindValue(
        toggleCompletionQueryId, ":uuid", QString::fromStdString(uuid));
    dbService.bindValue(toggleCompletionQueryId,
                        ":time_stamp",
                        utils::DateTimeConverter::qDateTime(timeStamp));
    dbService.executePrepared(toggleCompletionQueryId);
}

void QtTaskStorageWriter::updatePriorities(
    const std::vector<std::string>& priorities)
{
    dbService.requestTransaction();

    for (size_t i = 0; i < priorities.size(); ++i) {
        dbService.bindValue(updatePrioritiesQueryId,
                            ":uuid",
                            QString::fromStdString(priorities[i]));
        dbService.bindValue(
            updatePrioritiesQueryId, ":priority", static_cast<int>(i));
        dbService.executePrepared(updatePrioritiesQueryId);
    }

    dbService.commit();
}

void QtTaskStorageWriter::editTag(const std::string& oldName,
                                  const std::string& newName)
{
    dbService.bindValue(
        editTagQueryId, ":old_name", QString::fromStdString(oldName));
    dbService.bindValue(
        editTagQueryId, ":new_name", QString::fromStdString(newName));
    dbService.executePrepared(editTagQueryId);
}

} // namespace sprint_timer::storage::qt_storage_impl
