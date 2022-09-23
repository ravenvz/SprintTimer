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
#ifndef SPRINT_TIMER_APP_QTTASKSTORAGEREADERCONNECTIONPROXY_H
#define SPRINT_TIMER_APP_QTTASKSTORAGEREADERCONNECTIONPROXY_H

#include "ThreadConnectionHelper.h"
#include "api/TaskStorageReader.h"

namespace sprint_timer::compose {

class QtTaskStorageReaderConnectionProxy : public TaskStorageReader {
public:
    explicit QtTaskStorageReaderConnectionProxy(
        ThreadConnectionHelper& connectionHelper);

    auto unfinishedTasks() -> std::vector<Task> override;

    auto finishedTasks(const dw::DateRange& dateRange)
        -> std::vector<Task> override;

    auto allTasks(const dw::DateRange& dateRange) -> std::vector<Task> override;

    auto allTags() -> std::vector<std::string> override;

    auto findByUuid(const std::string& uuid) -> std::vector<Task> override;

    auto findMatching(std::span<const std::string> uuids)
        -> std::vector<Task> override;

    auto taskTree() -> TaskTree override;

private:
    ThreadConnectionHelper& connectionHelper;

    auto checkStorageInteractorInitialized() -> void;
};

} // namespace sprint_timer::compose

#endif // SPRINT_TIMER_APP_QTTASKSTORAGEREADERCONNECTIONPROXY_H
