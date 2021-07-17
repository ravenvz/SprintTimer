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
#ifndef ITASKSTORAGEREADER_H_RMTKEREJ
#define ITASKSTORAGEREADER_H_RMTKEREJ

#include "core/entities/Task.h"
#include "date_wrapper/date_wrapper.h"
#include <functional>

namespace sprint_timer {

class TaskStorageReader {
public:
    virtual ~TaskStorageReader() = default;

    virtual std::vector<entities::Task> unfinishedTasks() = 0;

    virtual std::vector<entities::Task>
    finishedTasks(const dw::DateRange& dateRange) = 0;

    virtual std::vector<entities::Task>
    allTasks(const dw::DateRange& dateRange) = 0;

    virtual std::vector<std::string> allTags() = 0;

    virtual std::vector<entities::Task> findByUuid(const std::string& uuid) = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: ITASKSTORAGEREADER_H_RMTKEREJ */
