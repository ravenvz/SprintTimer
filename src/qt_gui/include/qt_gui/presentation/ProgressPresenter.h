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
#ifndef DAILYPROGRESSPRESENTER_H_JN1XIWIZ
#define DAILYPROGRESSPRESENTER_H_JN1XIWIZ

#include "core/BackRequestStrategy.h"
#include "core/QueryHandler.h"
#include "core/use_cases/request_progress/RequestProgressQuery.h"
#include "core/utils/StringUtils.h"
#include "qt_gui/presentation/ProgressPresenterContract.h"
#include <optional>

namespace sprint_timer::ui {

class ProgressPresenter
    : public mvp::BasePresenter<contracts::DailyProgress::View> {
public:
    using request_progress_hdl_t =
        QueryHandler<use_cases::RequestProgressQuery>;

    explicit ProgressPresenter(request_progress_hdl_t& requestProgressHandler);

private:
    request_progress_hdl_t& requestProgressHandler;
    std::optional<request_progress_hdl_t::result_t> data;

    void fetchDataImpl() override;

    void updateViewImpl() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: DAILYPROGRESSPRESENTER_H_JN1XIWIZ */
