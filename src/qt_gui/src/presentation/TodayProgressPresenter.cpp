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
#include "qt_gui/presentation/TodayProgressPresenter.h"

namespace {

constexpr std::string_view title{"Daily goal progress: "};
constexpr std::string_view normalStyle = "QLabel { color: black; }";
constexpr std::string_view overworkStyle = "QLabel { color: red; }";
constexpr std::string_view matchStyle = "QLabel { color: green; }";

std::string pickStyle(const sprint_timer::GoalProgress& progress);

std::string constructProgressString(const sprint_timer::GoalProgress& progress);

} // namespace

namespace sprint_timer::ui {

TodayProgressPresenter::TodayProgressPresenter(
    request_progress_hdl_t& requestProgressHandler_)
    : requestProgressHandler{requestProgressHandler_}
{
}

void TodayProgressPresenter::fetchDataImpl()
{
    data = requestProgressHandler.handle(use_cases::RequestProgressQuery{});
}

void TodayProgressPresenter::updateViewImpl()
{
    if (auto v = view(); v && data) {
        if (data->size() == 0) {
            return;
        }
        const auto progress = data->getValue(data->size() - 1);

        const auto progressStr = constructProgressString(progress);
        const std::string style{pickStyle(progress)};

        v.value()->displayProgress(progressStr, style);
    }
}

} // namespace sprint_timer::ui

namespace {

std::string pickStyle(const sprint_timer::GoalProgress& progress)
{
    if (progress.estimated() == 0) {
        return "";
    }
    if (progress.actual() < progress.estimated()) {
        return std::string{normalStyle};
    }
    if (progress.actual() == progress.estimated()) {
        return std::string{matchStyle};
    }
    return std::string{overworkStyle};
}

std::string constructProgressString(const sprint_timer::GoalProgress& progress)
{
    if (progress.estimated() == 0) {
        return "";
    }
    std::string progressStr{title};
    progressStr += std::to_string(progress.actual());
    progressStr += "/";
    progressStr += std::to_string(progress.estimated());
    return progressStr;
}

} // namespace
