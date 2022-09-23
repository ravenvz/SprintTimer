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
#ifndef QUERYUTILS_H_145ABLDE
#define QUERYUTILS_H_145ABLDE

#include <QDateTime>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <optional>

namespace sprint_timer::storage::qt_storage {

auto tryExecute(QSqlQuery& query, const QString& queryStr) -> void;

auto tryExecute(QSqlQuery& query) -> void;

auto tryPrepare(const QString& connectionName, const QString& queryStr)
    -> QSqlQuery;

/* Executes query and fetches all records. Query is finalized. May throw
 * QueryError. */
auto tryFetchAll(QSqlQuery& query, const QString& queryStr)
    -> std::vector<QSqlRecord>;

/* Executes query and fetches all records. Query is finalized. May throw
 * QueryError. */
auto tryFetchAll(QSqlQuery& query) -> std::vector<QSqlRecord>;

auto copyAllRecords(QSqlQuery& query) -> std::vector<QSqlRecord>;

auto maybeString(const QSqlRecord& record, int column)
    -> std::optional<std::string>;

auto maybeBool(const QSqlRecord& record, int column) -> std::optional<bool>;

auto maybeInt(const QSqlRecord& record, int column) -> std::optional<int>;

auto maybeDateTime(const QSqlRecord& record, int column)
    -> std::optional<QDateTime>;

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QUERYUTILS_H_145ABLDE */
