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
#ifndef READTASKTREEQUERYHANDLER_H_GEKDPRZ1
#define READTASKTREEQUERYHANDLER_H_GEKDPRZ1

#include "api/com_query/QueryHandler.h"
#include "api/requests/ReadTaskTreeQuery.h"
#include "api/TaskStorageReader.h"
#include "core/TaskTreeMetadataReader.h"

namespace sprint_timer::api {

class ReadTaskTreeHandler : public asp::QueryHandler<ReadTaskTreeQuery> {
public:
    ReadTaskTreeHandler(TaskStorageReader& taskStorageReader,
                        TaskTreeMetadataReader& taskTreeMetadataReader);

    ReadTaskTreeQuery::Result handle(const ReadTaskTreeQuery& query) override;

private:
    TaskStorageReader& taskStorageReader;
    TaskTreeMetadataReader& taskTreeMetadataReader;
};

} // namespace sprint_timer::api

#endif /* end of include guard: READTASKTREEQUERYHANDLER_H_GEKDPRZ1 */

