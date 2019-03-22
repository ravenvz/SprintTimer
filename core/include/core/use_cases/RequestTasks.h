/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#ifndef SPRINT_TIMER_REQUESTTASKS_H
#define SPRINT_TIMER_REQUESTTASKS_H

#include "core/ITaskStorageReader.h"
#include "core/Query.h"
#include "date_wrapper/date_wrapper.h"

namespace sprint_timer::use_cases {

class RequestTasks : public Query {
public:
    RequestTasks(ITaskStorageReader& taskStorageReader,
                 const dw::DateRange& dateRange,
                 ITaskStorageReader::Handler handler);

    void execute() final;

    std::string describe() const final;

private:
    ITaskStorageReader& reader;
    const dw::DateRange dateRange_;
    ITaskStorageReader::Handler handler_;
};

} // namespace sprint_timer::use_cases

#endif // SPRINT_TIMER_REQUESTTASKS_H
