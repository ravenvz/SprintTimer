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
#ifndef UTILS_H_4MJTC1OS
#define UTILS_H_4MJTC1OS

#include "api/dtos/TaskDTO.h"
#include "date_wrapper/date_wrapper.h"
#include <filesystem>
#include <iostream>
#include <span>

std::vector<std::string>
extractUuids(const std::vector<sprint_timer::api::TaskDTO>& taskDTOs);

std::vector<std::string>
extractUuids(std::span<const sprint_timer::api::TaskDTO> taskDTOs);

constexpr dw::DateTimeRange createRange(dw::DateTime start) noexcept
{
    using namespace std::chrono_literals;
    return dw::DateTimeRange{start, start + 25min};
}

struct TempFile {
    explicit TempFile(std::filesystem::path file_);

    ~TempFile();

    // Calling this function prevents file deletion. Might be useful for
    // debugging.
    auto retain() -> void;

private:
    std::filesystem::path file;
    bool shouldBeRetained{false};
};

#endif /* end of include guard: UTILS_H_4MJTC1OS */
