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
#ifndef TODAYPROGRESSPRESENTER_H_31EAPJRM
#define TODAYPROGRESSPRESENTER_H_31EAPJRM

#include "core/AsyncQueryHandler.h"
#include "core/use_cases/request_progress/RequestProgressQuery.h"
#include "qt_gui/presentation/TodayProgressContract.h"
#include <optional>

namespace sprint_timer::ui {

class TodayProgressPresenter
    : public mvp::BasePresenter<contracts::TodayProgressContract::View> {
public:
    using request_progress_hdl_t =
        QueryHandler<use_cases::RequestProgressQuery>;

    explicit TodayProgressPresenter(
        request_progress_hdl_t& requestProgressHandler);

private:
    request_progress_hdl_t& requestProgressHandler;
    std::optional<request_progress_hdl_t::result_t> data;

    void fetchDataImpl() override;

    void updateViewImpl() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: TODAYPROGRESSPRESENTER_H_31EAPJRM */
