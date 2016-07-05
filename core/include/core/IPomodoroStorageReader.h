/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef IPOMODOROSTORAGEREADER_H_UJ8CZPXS
#define IPOMODOROSTORAGEREADER_H_UJ8CZPXS

#include "core/entities/Pomodoro.h"
#include <functional>


class IPomodoroStorageReader {
public:
    using Items = std::vector<Pomodoro>;
    using Handler = std::function<void(const Items&)>;
    virtual ~IPomodoroStorageReader() = default;
    virtual void requestItems(const TimeSpan& timeSpan, Handler handler) = 0;
};


#endif /* end of include guard: IPOMODOROSTORAGEREADER_H_UJ8CZPXS */
