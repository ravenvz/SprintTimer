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

#include "core/Task.h"
#include "core/TaskTree.h"
#include "date_wrapper/date_wrapper.h"
#include <functional>
#include <span>

namespace sprint_timer {

class TaskStorageReader {
public:
    virtual ~TaskStorageReader() = default;

    virtual auto unfinishedTasks() -> std::vector<Task> = 0;

    virtual auto finishedTasks(const dw::DateRange& dateRange)
        -> std::vector<Task> = 0;

    virtual auto allTasks(const dw::DateRange& dateRange)
        -> std::vector<Task> = 0;

    virtual auto allTags() -> std::vector<std::string> = 0;

    virtual auto findByUuid(const std::string& uuid) -> std::vector<Task> = 0;

    virtual auto findMatching(std::span<const std::string> uuids)
        -> std::vector<Task> = 0;

    virtual auto taskTree() -> TaskTree = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: ITASKSTORAGEREADER_H_RMTKEREJ */
