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
#include "core/TaskStorageReader.h"

namespace sprint_timer::compose {

class QtTaskStorageReaderConnectionProxy : public TaskStorageReader {
public:
    QtTaskStorageReaderConnectionProxy(
        ThreadConnectionHelper& connectionHelper);

    std::vector<entities::Task> unfinishedTasks() override;

    std::vector<entities::Task>
    finishedTasks(const dw::DateRange& dateRange) override;

    std::vector<entities::Task>
    allTasks(const dw::DateRange& dateRange) override;

    std::vector<std::string> allTags() override;

    std::vector<entities::Task> findByUuid(const std::string& uuid) override;

private:
    ThreadConnectionHelper& connectionHelper;

    void checkStorageInteractorInitialized();
};

} // namespace sprint_timer::compose

#endif // SPRINT_TIMER_APP_QTTASKSTORAGEREADERCONNECTIONPROXY_H
