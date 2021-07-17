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
#ifndef THREADCONNECTIONHELPER_H_VG4MQRJB
#define THREADCONNECTIONHELPER_H_VG4MQRJB

#include <QString>
#include <string>

namespace sprint_timer::compose {

class ThreadConnectionHelper {
public:
    ThreadConnectionHelper(std::string sqliteFileName);

    virtual ~ThreadConnectionHelper() = default;

    [[nodiscard]] QString connectionName();

    ThreadConnectionHelper(ThreadConnectionHelper&&) = delete;
    ThreadConnectionHelper& operator=(ThreadConnectionHelper&&) = delete;

    ThreadConnectionHelper(const ThreadConnectionHelper&) = delete;
    ThreadConnectionHelper& operator=(const ThreadConnectionHelper&) = delete;

private:
    std::string sqliteFileName;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: THREADCONNECTIONHELPER_H_VG4MQRJB */
