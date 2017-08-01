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
#ifndef ITASKSTORAGEREADER_H_RMTKEREJ
#define ITASKSTORAGEREADER_H_RMTKEREJ

#include "core/entities/Task.h"
#include "date_wrapper/TimeSpan.h"
#include <functional>

class ITaskStorageReader {
public:
    using Items = std::vector<Task>;

    using Handler = std::function<void(const Items&)>;

    using TagHandler = std::function<void(const std::vector<std::string>&)>;

    virtual ~ITaskStorageReader() = default;

    virtual void requestUnfinishedTasks(Handler handler) = 0;

    virtual void requestFinishedTasks(const dw::TimeSpan& timeSpan, Handler handler)
        = 0;

    virtual void requestTasks(const dw::TimeSpan& timeSpan, Handler handler) = 0;

    virtual void requestAllTags(TagHandler handler) = 0;
};

#endif /* end of include guard: ITASKSTORAGEREADER_H_RMTKEREJ */
