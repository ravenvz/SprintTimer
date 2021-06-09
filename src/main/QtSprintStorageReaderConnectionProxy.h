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
#ifndef QTSPRINTSTORAGEREADERCONNECTIONPROXY_H_NKO8EHDL
#define QTSPRINTSTORAGEREADERCONNECTIONPROXY_H_NKO8EHDL

#include "ThreadConnectionHelper.h"
#include "core/IStorageImplementersFactory.h"
#include "core/SprintStorageReader.h"
#include <QString>

namespace sprint_timer::compose {

class QtSprintStorageReaderConnectionProxy : public SprintStorageReader {
public:
    QtSprintStorageReaderConnectionProxy(ThreadConnectionHelper& connectionHelper);

    ~QtSprintStorageReaderConnectionProxy() = default;

    QtSprintStorageReaderConnectionProxy(
        QtSprintStorageReaderConnectionProxy&&) = delete;
    QtSprintStorageReaderConnectionProxy&
    operator=(QtSprintStorageReaderConnectionProxy&&) = delete;

    QtSprintStorageReaderConnectionProxy(
        const QtSprintStorageReaderConnectionProxy&) = delete;
    QtSprintStorageReaderConnectionProxy&
    operator=(const QtSprintStorageReaderConnectionProxy&) = delete;

    std::vector<entities::Sprint>
    findByDateRange(const dw::DateRange& dateRange) override;

    std::vector<entities::Sprint>
    findByTaskUuid(const std::string& taskUuid) override;

    std::vector<entities::Sprint> findByUuid(const std::string& uuid) override;

private:
    ThreadConnectionHelper& connectionHelper;

    void createStorageInteractor();
};

} // namespace sprint_timer::compose

#endif /* end of include guard:                                                \
          QTSPRINTSTORAGEREADERCONNECTIONPROXY_H_NKO8EHDL */
