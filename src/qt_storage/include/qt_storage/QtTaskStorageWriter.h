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
#ifndef QTTASKSTORAGEWRITER_H_AB4O73ZJ
#define QTTASKSTORAGEWRITER_H_AB4O73ZJ

#include "api/TaskStorageWriter.h"
#include "core/Tag.h"
#include "utils/DateTimeConverter.h"
#include <QSqlQuery>
#include <span>

namespace sprint_timer::storage::qt_storage {

class QtTaskStorageWriter : public api::TaskStorageWriter {
public:
    explicit QtTaskStorageWriter(QString connectionName);

    QtTaskStorageWriter(QtTaskStorageWriter&&) = delete;
    QtTaskStorageWriter& operator=(QtTaskStorageWriter&&) = delete;

    QtTaskStorageWriter(const QtTaskStorageWriter&) = delete;
    QtTaskStorageWriter& operator=(const QtTaskStorageWriter&) = delete;

    auto save(const Task& task) -> void final;

    auto remove(const std::string& uuid) -> void final;

    auto restore(const Task& task) -> void final;

    auto edit(const Task& oldTask, const Task& editedTask) -> void final;

    auto toggleCompleted(const std::string& uuid,
                         const dw::DateTime& timeStamp) -> void final;

    auto
    updatePriorities(const std::vector<std::string>& priorities) -> void final;

    auto editTag(const std::string& oldName,
                 const std::string& newName) -> void final;

    auto saveTree(const TaskTree& taskTree) -> void final;

    auto saveFullTree(const TaskTree& taskTree) -> void final;

private:
    QString connectionName;
    QSqlQuery createTaskQuery;
    QSqlQuery deleteTaskQuery;
    QSqlQuery restoreTaskQuery;
    QSqlQuery restoreSprintQuery;
    QSqlQuery deleteSprintsQuery;
    QSqlQuery createTagQuery;
    QSqlQuery deleteTagQuery;
    QSqlQuery editTaskQuery;
    QSqlQuery toggleCompletionQuery;
    QSqlQuery updatePrioritiesQuery;
    QSqlQuery editTagQuery;
    // QSqlQuery insertSprintQuery;
    QSqlQuery insertNotesQuery;
    QSqlQuery insertTimeframeQuery;
    QSqlQuery insertIntoTaskTreeQuery;
    utils::DateTimeConverter dateTimeConverter;

    auto insertTags(const QString& taskUuid, std::span<const Tag> tags) -> void;

    auto removeTags(const QString& taskUuid, std::span<const Tag> tags) -> void;

    // auto insertSprint(const QString& taskUuid, const Sprint& sprint) -> void;

    auto insertNotes(const QString& taskUuid, const Note& notes) -> void;

    auto insertTimeframe(const QString& taskUuid,
                         TaskTimeframe timeFrame) -> void;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QTTASKSTORAGEWRITER_H_AB4O73ZJ */
