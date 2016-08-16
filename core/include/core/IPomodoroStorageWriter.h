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
#ifndef IPOMODOROSTORAGEWRITER_H_PMJNRBFY
#define IPOMODOROSTORAGEWRITER_H_PMJNRBFY

#include "TimeSpan.h"
#include "core/entities/Sprint.h"


class IPomodoroStorageWriter {
public:
    virtual ~IPomodoroStorageWriter() = default;

    virtual void save(const Sprint& pomodoro) = 0;

    virtual void remove(const Sprint& pomodoro) = 0;
};


#endif /* end of include guard: IPOMODOROSTORAGEWRITER_H_PMJNRBFY */
