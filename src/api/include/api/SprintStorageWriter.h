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
#ifndef ISPRINTSTORAGEWRITER_H_PMJNRBFY
#define ISPRINTSTORAGEWRITER_H_PMJNRBFY

#include "core/Sprint.h"
#include "date_wrapper/date_wrapper.h"
#include <span>

namespace sprint_timer::api {

class SprintStorageWriter {
public:
    virtual ~SprintStorageWriter() = default;

    virtual void save(const std::string& taskUuid,
                      std::span<const Sprint> sprints) = 0;

    virtual void remove(const Sprint& sprint) = 0;

    virtual void remove(std::span<const Sprint> sprints) = 0;

    virtual void restore(const Sprint& sprint) = 0;
};

} // namespace sprint_timer::api

#endif /* end of include guard: ISPRINTSTORAGEWRITER_H_PMJNRBFY */
