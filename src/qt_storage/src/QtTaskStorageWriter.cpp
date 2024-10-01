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
#include "api/GatewayException.h"
#include "cpp_utils/algorithms/optional_ext.h"
#include "qt_storage/DatabaseDescription.h"
#include "qt_storage/TransactionGuard.h"
#include "qt_storage/utils.h"
#include "qt_storage/utils/DateTimeConverter.h"
#include "qt_storage/utils/QueryUtils.h"
#include <QVariant>
#include <algorithm>
#include <fstream>

namespace sprint_timer::storage::qt_storage {

using namespace qt_storage;

QtTaskStorageWriter::QtTaskStorageWriter(QString connectionName_)
    : connectionName{std::move(connectionName_)}
{
    createTaskQuery =
        tryPrepare(connectionName,
                   QString{"INSERT INTO %1 (%2, %3, %4, %5, %6, %7) "
                           "VALUES (:uuid, :name, :estimated_cost, "
                           ":completed, :last_modified, :type);"}
                       .arg(TaskTable::name)
                       .arg(TaskTable::Columns::uuid)
                       .arg(TaskTable::Columns::name)
                       .arg(TaskTable::Columns::estimatedCost)
                       .arg(TaskTable::Columns::completed)
                       .arg(TaskTable::Columns::lastModified)
                       .arg(TaskTable::Columns::type));
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
    // createTaskQuery =
    //     tryPrepare(connectionName,
    //                QString{"INSERT INTO %1 (%2, %3, %4, %5, %6) "
    //                        "VALUES (:uuid, :name, :estimated_cost, "
    //                        ":completed, :last_modified) "
    //                        "ON CONFLICT(%2) DO "
    //                        "UPDATE SET %7 = 0;"}
    //                    .arg(TaskTable::name)
    //                    .arg(TaskTable::Columns::uuid)
    //                    .arg(TaskTable::Columns::name)
    //                    .arg(TaskTable::Columns::estimatedCost)
    //                    .arg(TaskTable::Columns::completed)
    //                    .arg(TaskTable::Columns::lastModified)
    //                    .arg(TaskTable::Columns::deleted));
    // deleteTaskQuery = tryPrepare(connectionName,
    //                              QString{"UPDATE %1 SET %2 = 1 WHERE %3 =
    //                              :uuid;"}
    //                                  .arg(TaskTable::name)
    //                                  .arg(TaskTable::Columns::deleted)
    //                                  .arg(TaskTable::Columns::uuid));
    deleteTaskQuery = tryPrepare(connectionName,
                                 QString{"DELETE FROM %1 WHERE %2 = :uuid;"}
                                     .arg(TaskTable::name)
                                     .arg(TaskTable::Columns::uuid));
    // editTaskQuery = tryPrepare(connectionName,
    //                            QString{"UPDATE %1 SET %2 = :name, "
    //                                    "%3 = :estimated_cost, "
    //                                    "%4 = :last_modified, "
    //                                    "%5 = :type "
    //                                    "WHERE %6 = :uuid;"}
    //                                .arg(TaskTable::name)
    //                                .arg(TaskTable::Columns::name)
    //                                .arg(TaskTable::Columns::estimatedCost)
    //                                .arg(TaskTable::Columns::lastModified)
    //                                .arg(TaskTable::Columns::type)
    //                                .arg(TaskTable::Columns::uuid));
    editTaskQuery = tryPrepare(connectionName,
                               QString{"UPDATE %1 SET "
                                       "%2 = :name, "
                                       "%3 = :estimated_cost, "
                                       "%4 = :completed, "
                                       "%5 = :last_modified, "
                                       "%6 = :text, "
                                       "%7 = :start, "
                                       "%8 = :due, "
                                       "%9 = :reminder, "
                                       "%10 = :recurrence, "
                                       "%11 = :type "
                                       "WHERE %12 = :uuid;"}
                                   .arg(AdvTaskView::name)
                                   .arg(TaskTable::Columns::name)
                                   .arg(TaskTable::Columns::estimatedCost)
                                   .arg(TaskTable::Columns::completed)
                                   .arg(TaskTable::Columns::lastModified)
                                   .arg(NotesTable::Columns::text)
                                   .arg(TaskTimeframeTable::Columns::start)
                                   .arg(TaskTimeframeTable::Columns::due)
                                   .arg(TaskTimeframeTable::Columns::reminder)
                                   .arg(TaskTimeframeTable::Columns::recurrence)
                                   .arg(TaskTable::Columns::type)
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
    insertIntoTaskTreeQuery =
        tryPrepare(connectionName,
                   QString{"INSERT INTO %1 (%2) "
                           "VALUES (:uuid);"}
                       .arg(TaskTreeTable::name)
                       .arg(TaskTreeTable::Columns::task_uuid));
}

auto QtTaskStorageWriter::save(const Task& task) -> void
{
    const QString uuid = QString::fromStdString(task.uuid());
    createTaskQuery.bindValue(":name", QString::fromStdString(task.name()));
    createTaskQuery.bindValue(":estimated_cost", task.estimatedCost());
    createTaskQuery.bindValue(":actual_cost", task.actualCost());
    createTaskQuery.bindValue(":completed", task.isCompleted());
    createTaskQuery.bindValue(":priority", 10000);
    createTaskQuery.bindValue(":type", static_cast<int>(task.kind()));
    createTaskQuery.bindValue(":last_modified",
                              dateTimeConverter(task.lastModified()));
    createTaskQuery.bindValue(":uuid", uuid);

    TransactionGuard guard{connectionName};
    tryExecute(createTaskQuery);
    insertTags(uuid, task.tags());

    alg::inspect(task.notes(),
                 [&](const auto& note) { insertNotes(uuid, note); });
    insertTimeframe(uuid, task.timeFrame());
    for (const auto& sprint : task.sprints()) {
        insertSprint(uuid, sprint);
    }
    guard.commit();
}

auto QtTaskStorageWriter::remove(const std::string& uuid) -> void
{
    // Note that tags are removed by sqlite trigger
    deleteTaskQuery.bindValue(":uuid", QString::fromStdString(uuid));
    tryExecute(deleteTaskQuery);
}

auto QtTaskStorageWriter::edit(const Task& oldTask,
                               const Task& editedTask) -> void
{
    const QString taskUuid = QString::fromStdString(oldTask.uuid());

    editTaskQuery.bindValue(":name", QString::fromStdString(editedTask.name()));
    editTaskQuery.bindValue(":estimated_cost", editedTask.estimatedCost());
    editTaskQuery.bindValue(":last_modified",
                            dateTimeConverter(editedTask.lastModified()));
    editTaskQuery.bindValue(":uuid", taskUuid);
    editTaskQuery.bindValue(":completed", editedTask.isCompleted());
    editTaskQuery.bindValue(
        ":text",
        editedTask.notes()
            .transform([](const auto& note) {
                return QVariant{QString::fromStdString(note.textNotes())};
            })
            .value_or(QVariant{}));
    editTaskQuery.bindValue(":start",
                            dateTimeConverter(editedTask.activeSince()));
    editTaskQuery.bindValue(":due",
                            editedTask.dueTo()
                                .transform([this](const auto& dateTime) {
                                    return QVariant{
                                        dateTimeConverter(dateTime)};
                                })
                                .value_or(QVariant{}));
    editTaskQuery.bindValue(":reminder",
                            editedTask.remindAt()
                                .transform([this](const auto& dateTime) {
                                    return QVariant{
                                        dateTimeConverter(dateTime)};
                                })
                                .value_or(QVariant{}));
    editTaskQuery.bindValue(
        ":recurrence",
        editedTask.recurrence()
            .transform([](const auto& recurrence) {
                return QVariant{QString::fromStdString(recurrence.pattern())};
            })
            .value_or(QVariant{}));

    editTaskQuery.bindValue(":type", static_cast<int>(editedTask.kind()));

    std::vector<Tag> oldTags{cbegin(oldTask.tags()), cend(oldTask.tags())};
    std::vector<Tag> newTags{cbegin(editedTask.tags()),
                             cend(editedTask.tags())};
    std::ranges::sort(oldTags);
    std::ranges::sort(newTags);
    std::vector<Tag> tagsToRemove;
    std::vector<Tag> tagsToInsert;

    utils::twoWayDiff(cbegin(oldTags),
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

auto QtTaskStorageWriter::toggleCompleted(const std::string& uuid,
                                          const dw::DateTime& timeStamp) -> void
{
    toggleCompletionQuery.bindValue(":uuid", QString::fromStdString(uuid));
    toggleCompletionQuery.bindValue(":time_stamp",
                                    dateTimeConverter(timeStamp));
    tryExecute(toggleCompletionQuery);
}

auto QtTaskStorageWriter::updatePriorities(
    const std::vector<std::string>& priorities) -> void
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

auto QtTaskStorageWriter::editTag(const std::string& oldName,
                                  const std::string& newName) -> void
{
    editTagQuery.bindValue(":old_name", QString::fromStdString(oldName));
    editTagQuery.bindValue(":new_name", QString::fromStdString(newName));
    tryExecute(editTagQuery);
}

auto QtTaskStorageWriter::insertTags(const QString& taskUuid,
                                     std::span<const Tag> tags) -> void
{
    for (const auto& tag : tags) {
        createTagQuery.bindValue(":tag", QString::fromStdString(tag.name()));
        createTagQuery.bindValue(":uuid", taskUuid);
        tryExecute(createTagQuery);
    }
}

auto QtTaskStorageWriter::removeTags(const QString& taskUuid,
                                     std::span<const Tag> tags) -> void
{
    for (const auto& tag : tags) {
        deleteTagQuery.bindValue(":uuid", taskUuid);
        deleteTagQuery.bindValue(":tag", QString::fromStdString(tag.name()));
        tryExecute(deleteTagQuery);
    }
}

auto QtTaskStorageWriter::saveTree(const TaskTree& taskTree) -> void
{
    const auto flattenedTree = taskTree.flatten();

    TransactionGuard guard{connectionName};

    QSqlQuery query{QSqlDatabase::database(connectionName)};
    tryExecute(query, QString{"DELETE FROM %1;"}.arg(TaskTreeTable::name));

    for (const auto& node : flattenedTree) {
        insertIntoTaskTreeQuery.bindValue(
            ":uuid", node ? QString::fromStdString(node->uuid()) : QVariant{});
        tryExecute(insertIntoTaskTreeQuery);
    }

    guard.commit();
}

auto QtTaskStorageWriter::saveFullTree(const TaskTree& taskTree) -> void { }

auto QtTaskStorageWriter::insertSprint(
    const QString& taskUuid, const sprint_timer::Sprint& sprint) -> void
{
    const QDateTime startTime = dateTimeConverter(sprint.start());
    const QDateTime finishTime = dateTimeConverter(sprint.finish());
    insertSprintQuery.bindValue(":todo_uuid", QVariant(taskUuid));
    insertSprintQuery.bindValue(":startTime", QVariant(startTime));
    insertSprintQuery.bindValue(":finishTime", QVariant(finishTime));
    tryExecute(insertSprintQuery);
}

auto QtTaskStorageWriter::insertNotes(const QString& taskUuid,
                                      const Note& notes) -> void
{
    insertNotesQuery.bindValue(":task_uuid", QVariant(taskUuid));
    insertNotesQuery.bindValue(
        ":text", QVariant(QString::fromStdString(notes.textNotes())));
    tryExecute(insertNotesQuery);
}

auto QtTaskStorageWriter::insertTimeframe(const QString& taskUuid,
                                          TaskTimeframe timeFrame) -> void
{
    insertTimeframeQuery.bindValue(":task_uuid", QVariant(taskUuid));
    insertTimeframeQuery.bindValue(
        ":start", QVariant(dateTimeConverter(timeFrame.start)));
    insertTimeframeQuery.bindValue(":due",
                                   timeFrame.due
                                       .transform([&](const auto& dateTime) {
                                           return QVariant(
                                               dateTimeConverter(dateTime));
                                       })
                                       .value_or(QVariant{}));
    alg::inspect(timeFrame.remindAt, [&](dw::DateTime remind) {
        insertTimeframeQuery.bindValue(":reminder",
                                       QVariant(dateTimeConverter(remind)));
    });
    alg::inspect(timeFrame.recurrence, [&](const Recurrence& recurrence) {
        insertTimeframeQuery.bindValue(
            ":recurrence",
            QVariant(QString::fromStdString(recurrence.pattern())));
    });
    tryExecute(insertTimeframeQuery);
}

} // namespace sprint_timer::storage::qt_storage
