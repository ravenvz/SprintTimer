/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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
#ifndef FAKETASKSTORAGEREADER_H_EDPTGODR
#define FAKETASKSTORAGEREADER_H_EDPTGODR

#include "FakeStorage.h"
#include "core/ITaskStorageReader.h"
#include <algorithm>

class FakeTaskStorageReader : public ITaskStorageReader {
public:
    FakeTaskStorageReader(FakeStorage<Task>& storage)
        : storage{storage}
    {
    }

    void requestUnfinishedTasks(Handler handler) final
    {
        auto tempHandler = [handler](const std::vector<Task>& allReturned) {
            std::vector<Task> unfinished;
            std::copy_if(allReturned.cbegin(), allReturned.cend(),
                         std::back_inserter(unfinished),
                         [](const auto& task) { return !task.isCompleted(); });
            handler(unfinished);
        };
        storage.requestUnfinishedItems(tempHandler);
    }

    void requestFinishedTasks(const dw::TimeSpan& timeSpan, Handler handler) final
    {
        auto tempHandler = [handler](const std::vector<Task>& allReturned) {
            std::vector<Task> completed;
            std::copy_if(allReturned.cbegin(), allReturned.cend(),
                    std::back_inserter(completed),
                    [](const auto& task) { return task.isCompleted(); });
            handler(completed);
        };
        storage.itemsInTimeRange(timeSpan, tempHandler);
    }

    void requestTasks(const dw::TimeSpan& timeSpan, Handler handler) final
    {
        storage.itemsInTimeRange(timeSpan, handler);
    }

    void requestAllTags(TagHandler handler)
    {
        const std::vector<std::string> emptyResult;
        handler(emptyResult);
    }

private:
    FakeStorage<Task>& storage;
};

#endif /* end of include guard: FAKETASKSTORAGEREADER_H_EDPTGODR */
