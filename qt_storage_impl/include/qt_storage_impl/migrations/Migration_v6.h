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
#ifndef MIGRATION_V6_H_FDXCK7SZ
#define MIGRATION_V6_H_FDXCK7SZ

#include "qt_storage_impl/migrations/Migration.h"

namespace sprint_timer::storage::qt_storage_impl {

class Migration_v6 final : public Migration {
public:
    void run(QSqlDatabase& database) const final;
};

} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: MIGRATION_V6_H_FDXCK7SZ */
