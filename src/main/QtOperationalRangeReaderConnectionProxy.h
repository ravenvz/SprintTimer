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
#ifndef QTOPERATIONALRANGEREADERCONNECTIONPROXY_H_CA3RLXBQ
#define QTOPERATIONALRANGEREADERCONNECTIONPROXY_H_CA3RLXBQ

#include "ThreadConnectionHelper.h"
#include "api/DateTimeProvider.h"
#include "api/OperationalRangeReader.h"

namespace sprint_timer::compose {

class QtOperationalRangeReaderConnectionProxy
    : public api::OperationalRangeReader {
public:
    QtOperationalRangeReaderConnectionProxy(
        ThreadConnectionHelper& connectionHelper,
        const api::DateTimeProvider& dateTimeProvider);

    auto operationalRange() -> dw::DateRange override;

private:
    ThreadConnectionHelper& connectionHelper;
    const api::DateTimeProvider& dateTimeProvider;

    void checkStorageInteractorInitialized();
};

} // namespace sprint_timer::compose

#endif /* end of include guard:                                                \
          QTOPERATIONALRANGEREADERCONNECTIONPROXY_H_CA3RLXBQ */
