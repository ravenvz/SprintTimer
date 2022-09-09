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
#ifndef NOTEDTO_H_LMJ41ITP
#define NOTEDTO_H_LMJ41ITP

#include <string>
#include <vector>

namespace sprint_timer::api {

struct NoteDTO {
    std::string text;

    friend bool operator==(const NoteDTO&, const NoteDTO&) = default;
};

} // namespace sprint_timer::api

#endif /* end of include guard: NOTEDTO_H_LMJ41ITP */
