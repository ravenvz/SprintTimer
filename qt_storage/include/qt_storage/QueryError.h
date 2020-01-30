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
#ifndef QUERYERROR_H_WQUWETDP
#define QUERYERROR_H_WQUWETDP

#include <QSqlQuery>
#include <stdexcept>

namespace sprint_timer::storage::qt_storage {

class QueryError : public std::runtime_error {
public:
    QueryError(const std::string& description, const QSqlQuery& query);

    std::string queryText() const;

    std::string queryError() const;

private:
    const std::string query;
    const std::string lastError;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QUERYERROR_H_WQUWETDP */
