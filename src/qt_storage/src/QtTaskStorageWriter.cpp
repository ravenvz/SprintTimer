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
#include "core/utils/Algutils.h"
#include "qt_storage/DatabaseDescription.h"
#include "qt_storage/TransactionGuard.h"
#include "qt_storage/utils.h"
#include "qt_storage/utils/DateTimeConverter.h"
#include "qt_storage/utils/QueryUtils.h"
#include <QVariant>
#include <algorithm>

namespace sprint_timer::storage::qt_storage {

using namespace qt_storage;

QtTaskStorageWriter::QtTaskStorageWriter(QString connectionName_)
    : connectionName{std::move(connectionName_)}
{
    createTaskQuery =
        tryPrepare(connectionName,
                   QString{"INSERT INTO %1 (%2, %3, %4, %5, %6) "
                           "VALUES (:uuid, :name, :estimated_cost, "
                           ":completed, :last_modified);"}
                       .arg(TaskTable::name)
                       .arg(TaskTable::Columns::uuid)
                       .arg(TaskTable::Columns::name)
                       .arg(TaskTable::Columns::estimatedCost)
                       .arg(TaskTable::Columns::completed)
                       .arg(TaskTable::Columns::lastModified));
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
    editTagQuery = tryPrepare(connectionName,
                              QString{"UPDATE %1 SET %2 = :new_name WHERE %2 = "
                                      ":old_name;"}
                                  .arg(TagTable::name)
                                  .arg(TagTable::Columns::name));
    insertSprintQuery =
        tryPrepare(connectionName,
                   QString{"INSERT INTO %1 (%2, %3, %4) "
                           "VALUES("
                           "(SELECT %5 FROM %6 WHERE %7 = :todo_uuid), "
                           ":startTime, :finishTime);"}
                       .arg(SprintView::name)
                       .arg(SprintView::Aliases::taskid)
                       .arg(SprintTable::Columns::startTime)
                       .arg(SprintTable::Columns::finishTime)
                       .arg(TaskTable::Columns::id)
                       .arg(TaskTable::name)
                       .arg(TaskTable::Columns::uuid));
    insertNotesQuery =
        tryPrepare(connectionName,
                   QString{"INSERT INTO %1 (%2, %3) "
                           "VALUES((SELECT %4 FROM %5 WHERE %6 = :task_uuid), "
                           ":text);"}
                       .arg(NotesTable::name)
                       .arg(NotesTable::Columns::task_id)
                       .arg(NotesTable::Columns::text)
                       .arg(TaskTable::Columns::id)
                       .arg(TaskTable::name)
                       .arg(TaskTable::Columns::uuid));
    insertTimeframeQuery =
        tryPrepare(connectionName,
                   QString{"INSERT INTO %1 (%2, %3, %4, %5, %6) "
                           "VALUES ((SELECT %7 FROM %8 WHERE %9 = :task_uuid), "
                           ":start, :due, :reminder, :recurrence);"}
                       .arg(TaskTimeframeTable::name)
                       .arg(TaskTimeframeTable::Columns::task_id)
                       .arg(TaskTimeframeTable::Columns::start)
                       .arg(TaskTimeframeTable::Columns::due)
                       .arg(TaskTimeframeTable::Columns::reminder)
                       .arg(TaskTimeframeTable::Columns::recurrence)
                       .arg(TaskTable::Columns::id)
                       .arg(TaskTable::name)
                       .arg(TaskTable::Columns::uuid));
}

void QtTaskStorageWriter::save(const Task& task)
{
    using sprint_timer::utils::inspect;

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

    inspect(task.notes(), [&](const auto& note) { insertNotes(uuid, note); });
    inspect(task.timeFrame(),
            [&](const auto& frame) { insertTimeframe(uuid, frame); });
    for (const auto& sprint : task.sprints()) {
        insertSprint(uuid, sprint);
    }
    guard.commit();
}

void QtTaskStorageWriter::remove(const std::string& uuid)
{
    // Note that tags are removed by sqlite trigger
    deleteTaskQuery.bindValue(":uuid", QString::fromStdString(uuid));
    tryExecute(deleteTaskQuery);
}

void QtTaskStorageWriter::edit(const Task& oldTask, const Task& editedTask)
{
    using namespace utils;

    const QString taskUuid = QString::fromStdString(oldTask.uuid());

    editTaskQuery.bindValue(":name", QString::fromStdString(editedTask.name()));
    editTaskQuery.bindValue(":estimated_cost", editedTask.estimatedCost());
    editTaskQuery.bindValue(
        ":last_modified",
        DateTimeConverter::qDateTime(editedTask.lastModified()));
    editTaskQuery.bindValue(":uuid", taskUuid);

    std::vector<Tag> oldTags{cbegin(oldTask.tags()), cend(oldTask.tags())};
    std::vector<Tag> newTags{cbegin(editedTask.tags()),
                             cend(editedTask.tags())};
    std::ranges::sort(oldTags);
    std::ranges::sort(newTags);
    std::vector<Tag> tagsToRemove;
    std::vector<Tag> tagsToInsert;

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
                                     std::span<const Tag> tags)
{
    for (const auto& tag : tags) {
        createTagQuery.bindValue(":tag", QString::fromStdString(tag.name()));
        createTagQuery.bindValue(":uuid", taskUuid);
        tryExecute(createTagQuery);
    }
}

void QtTaskStorageWriter::removeTags(const QString& taskUuid,
                                     std::span<const Tag> tags)
{
    for (const auto& tag : tags) {
        deleteTagQuery.bindValue(":uuid", taskUuid);
        deleteTagQuery.bindValue(":tag", QString::fromStdString(tag.name()));
        tryExecute(deleteTagQuery);
    }
}

void QtTaskStorageWriter::insertSprint(const QString& taskUuid,
                                       const sprint_timer::Sprint& sprint)
{
    using storage::utils::DateTimeConverter;
    const QDateTime startTime = DateTimeConverter::qDateTime(sprint.start());
    const QDateTime finishTime = DateTimeConverter::qDateTime(sprint.finish());
    insertSprintQuery.bindValue(":todo_uuid", QVariant(taskUuid));
    insertSprintQuery.bindValue(":startTime", QVariant(startTime));
    insertSprintQuery.bindValue(":finishTime", QVariant(finishTime));
    tryExecute(insertSprintQuery);
}

void QtTaskStorageWriter::insertNotes(const QString& taskUuid,
                                      const Note& notes)
{
    insertNotesQuery.bindValue(":task_uuid", QVariant(taskUuid));
    insertNotesQuery.bindValue(
        ":text", QVariant(QString::fromStdString(notes.textNotes())));
    tryExecute(insertNotesQuery);
}

void QtTaskStorageWriter::insertTimeframe(const QString& taskUuid,
                                          TaskTimeframe timeFrame)
{
    using sprint_timer::utils::inspect;
    using utils::DateTimeConverter;
    insertTimeframeQuery.bindValue(":task_uuid", QVariant(taskUuid));
    insertTimeframeQuery.bindValue(
        ":start",
        QVariant(DateTimeConverter::qDateTime(timeFrame.frame.start())));
    insertTimeframeQuery.bindValue(
        ":due",
        QVariant(DateTimeConverter::qDateTime(timeFrame.frame.finish())));
    inspect(timeFrame.remindAt, [&](dw::DateTime remind) {
        insertTimeframeQuery.bindValue(
            ":reminder", QVariant(DateTimeConverter::qDateTime(remind)));
    });
    inspect(timeFrame.recurrence, [&](const Recurrence& recurrence) {
        insertTimeframeQuery.bindValue(
            ":recurrence",
            QVariant(QString::fromStdString(recurrence.pattern())));
    });
    tryExecute(insertTimeframeQuery);
}

} // namespace sprint_timer::storage::qt_storage
