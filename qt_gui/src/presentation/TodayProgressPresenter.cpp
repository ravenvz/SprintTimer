/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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

constexpr char const* title{"Daily goal progress: "};
constexpr char const* normalStyle = "QLabel { color: black; }";
constexpr char const* overworkStyle = "QLabel { color: red; }";
constexpr char const* matchStyle = "QLabel { color: green; }";

std::string pickStyle(const sprint_timer::GoalProgress& progress);

std::string constructProgressString(const sprint_timer::GoalProgress& progress);

} // namespace

namespace sprint_timer::ui {

TodayProgressPresenter::TodayProgressPresenter(
    request_progress_hdl_t& requestProgressHandler_)
    : requestProgressHandler{requestProgressHandler_}
{
}

void TodayProgressPresenter::updateViewImpl()
{
    if (!view) {
        return;
    }

    const auto progressOverPeriod =
        requestProgressHandler.handle(use_cases::RequestProgressQuery{});
    const auto progress =
        progressOverPeriod.getValue(progressOverPeriod.size() - 1);

    const auto progressStr = constructProgressString(progress);
    const std::string style{pickStyle(progress)};

    view->displayProgress(progressStr, style);
}

} // namespace sprint_timer::ui

namespace {

std::string pickStyle(const sprint_timer::GoalProgress& progress)
{
    if (progress.estimated() == 0) {
        return "";
    }
    if (progress.actual() < progress.estimated()) {
        return normalStyle;
    }
    if (progress.actual() == progress.estimated()) {
        return matchStyle;
    }
    return overworkStyle;
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
