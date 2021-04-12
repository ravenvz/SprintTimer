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
#include "qt_gui/presentation/WorkScheduleEditorPresenter.h"
#include "core/utils/StringUtils.h"
#include <numeric>

namespace {

constexpr uint16_t daysInWeek{7};

std::string weekScheduleToString(const sprint_timer::WeekSchedule& weekSchedule,
                                 dw::Weekday firstDayOfWeek);

template <typename ForwardIt>
void reorderByFirstWeekday(ForwardIt first,
                           ForwardIt last,
                           dw::Weekday firstDayOfWeek);

sprint_timer::WeekSchedule scheduleFromGoals(std::vector<uint8_t>&& goals,
                                             dw::Weekday firstDayOfWeek);

void displayWeekSchedule(
    const sprint_timer::WeekSchedule& weekSchedule,
    dw::Weekday firstDayOfWeek,
    sprint_timer::ui::contracts::WorkScheduleEditor::View* view);

std::vector<sprint_timer::ui::contracts::WorkScheduleEditor::DayAndGoal>
goalsByWeekday(dw::Weekday firstDayOfWeek,
               const sprint_timer::WeekSchedule& weekSchedule);

std::vector<std::pair<dw::Weekday, int>>
dailySchedule(const sprint_timer::WeekSchedule& weekSchedule,
              dw::Weekday firstDayOfWeek);

void displayRoaster(
    const sprint_timer::WorkSchedule::Roaster& roaster,
    dw::Weekday firstDayOfWeek,
    sprint_timer::ui::contracts::WorkScheduleEditor::View* view);

void displayExceptionalDays(
    std::vector<sprint_timer::WorkSchedule::DateGoal>&& exceptionalDays,
    sprint_timer::ui::contracts::WorkScheduleEditor::View* view);

} // namespace

namespace sprint_timer::ui {

WorkScheduleEditorPresenter::WorkScheduleEditorPresenter(
    QueryHandler<use_cases::WorkScheduleQuery, WorkSchedule>&
        workScheduleHandler_,
    CommandHandler<use_cases::ChangeWorkScheduleCommand>&
        changeWorkScheduleHandler_,
    dw::Weekday firstDayOfWeek_)
    : workScheduleHandler{workScheduleHandler_}
    , changeWorkScheduleHandler{changeWorkScheduleHandler_}
    , firstDayOfWeek{firstDayOfWeek_}
{
}

void WorkScheduleEditorPresenter::updateViewImpl()
{
    if (auto v = view(); v) {
        bufferedSchedule =
            workScheduleHandler.get().handle(use_cases::WorkScheduleQuery{});
        displayWeekSchedule(
            bufferedSchedule.currentWeekSchedule(), firstDayOfWeek, v.value());
        displayRoaster(bufferedSchedule.roaster(), firstDayOfWeek, v.value());
        displayExceptionalDays(bufferedSchedule.exceptionalDays(), v.value());
    }
}

void WorkScheduleEditorPresenter::onAddExceptionalRequested()
{
    if (auto v = view(); v) {
        v.value()->displayAddExceptionalDaysDialog(firstDayOfWeek,
                                                   dw::current_date_local());
    }
}

void WorkScheduleEditorPresenter::onExceptionalDaysAdded(dw::Date startDate,
                                                         int32_t numDays,
                                                         int32_t sprintsPerDay)
{
    if (auto v = view(); v) {
        for (int32_t i = 0; i < numDays; ++i) {
            bufferedSchedule.addExceptionalDay(startDate + dw::Days{i},
                                               sprintsPerDay);
        }
        displayExceptionalDays(bufferedSchedule.exceptionalDays(), v.value());
    }
}

void WorkScheduleEditorPresenter::onExceptionalDayRemoved(dw::Date date)
{
    bufferedSchedule.removeExceptionalDay(date);
    // displayExceptionalDays(bufferedSchedule.exceptionalDays(), view);
}

void WorkScheduleEditorPresenter::onWeekScheduleAdded(
    std::vector<uint8_t>&& schedule, dw::Date startDate)
{
    if (auto v = view(); v) {
        bufferedSchedule.addWeekSchedule(
            startDate, scheduleFromGoals(std::move(schedule), firstDayOfWeek));
        displayRoaster(bufferedSchedule.roaster(), firstDayOfWeek, v.value());
    }
}

void WorkScheduleEditorPresenter::onWeekScheduleRemoved(
    dw::Date scheduleStartDate)
{
    if (auto v = view(); v) {
        bufferedSchedule.removeWeekSchedule(scheduleStartDate);
        displayRoaster(bufferedSchedule.roaster(), firstDayOfWeek, v.value());
        displayWeekSchedule(
            bufferedSchedule.currentWeekSchedule(), firstDayOfWeek, v.value());
    }
}

void WorkScheduleEditorPresenter::onScheduleChangeConfirmed()
{
    const auto oldSchedule =
        workScheduleHandler.get().handle(use_cases::WorkScheduleQuery{});
    if (oldSchedule == bufferedSchedule)
        return;
    changeWorkScheduleHandler.get().handle(
        use_cases::ChangeWorkScheduleCommand{oldSchedule, bufferedSchedule});
}

void WorkScheduleEditorPresenter::onRevertChanges()
{
    updateView();
    // bufferedSchedule =
    //     workScheduleHandler.handle(use_cases::WorkScheduleQuery{});
}

} // namespace sprint_timer::ui

namespace {

sprint_timer::WeekSchedule scheduleFromGoals(std::vector<uint8_t>&& goals,
                                             dw::Weekday firstDayOfWeek)
{
    sprint_timer::WeekSchedule schedule;
    std::rotate(begin(goals),
                begin(goals) + daysInWeek - static_cast<int>(firstDayOfWeek),
                end(goals));
    for (size_t i = 0; i < goals.size(); ++i)
        schedule.setTargetGoal(static_cast<dw::Weekday>(i), goals[i]);
    return schedule;
}

std::string weekScheduleToString(const sprint_timer::WeekSchedule& weekSchedule,
                                 dw::Weekday firstDayOfWeek)
{
    auto goals = dailySchedule(weekSchedule, firstDayOfWeek);
    std::vector<std::string> strGoals(daysInWeek, "");
    std::transform(
        cbegin(goals), cend(goals), begin(strGoals), [](const auto& elem) {
            return std::to_string(elem.second);
        });
    return sprint_timer::utils::join(strGoals, ", ");
}

std::vector<std::pair<dw::Weekday, int>>
dailySchedule(const sprint_timer::WeekSchedule& weekSchedule,
              dw::Weekday firstDayOfWeek)
{
    std::vector<int> dayNumbers(daysInWeek, 0);
    std::iota(begin(dayNumbers), end(dayNumbers), 0);
    std::vector<std::pair<dw::Weekday, int>> result(daysInWeek);
    std::transform(
        cbegin(dayNumbers),
        cend(dayNumbers),
        begin(result),
        [&weekSchedule](auto day) -> std::pair<dw::Weekday, uint16_t> {
            const auto weekday = static_cast<dw::Weekday>(day);
            return {weekday, weekSchedule.targetGoal(weekday)};
        });
    reorderByFirstWeekday(begin(result), end(result), firstDayOfWeek);
    return result;
}

template <typename ForwardIt>
void reorderByFirstWeekday(ForwardIt first,
                           ForwardIt last,
                           dw::Weekday firstDayOfWeek)
{
    std::rotate(first, first + static_cast<int>(firstDayOfWeek), last);
}

void displayWeekSchedule(
    const sprint_timer::WeekSchedule& weekSchedule,
    dw::Weekday firstDayOfWeek,
    sprint_timer::ui::contracts::WorkScheduleEditor::View* view)
{
    const auto s = goalsByWeekday(firstDayOfWeek, weekSchedule);
    view->displayCurrentWeekSchedule(s);
}

std::vector<sprint_timer::ui::contracts::WorkScheduleEditor::DayAndGoal>
goalsByWeekday(dw::Weekday firstDayOfWeek,
               const sprint_timer::WeekSchedule& weekSchedule)
{
    using namespace sprint_timer::ui::contracts::WorkScheduleEditor;
    auto goals = dailySchedule(weekSchedule, firstDayOfWeek);
    std::vector<DayAndGoal> result(daysInWeek, {0, 0});
    std::transform(
        cbegin(goals),
        cend(goals),
        begin(result),
        [](const auto& elem)
            -> sprint_timer::ui::contracts::WorkScheduleEditor::DayAndGoal {
            return {static_cast<uint16_t>(elem.first) + 1, elem.second};
        });
    return result;
}

void displayRoaster(const sprint_timer::WorkSchedule::Roaster& roaster,
                    dw::Weekday firstDayOfWeek,
                    sprint_timer::ui::contracts::WorkScheduleEditor::View* view)
{
    using namespace sprint_timer::ui::contracts::WorkScheduleEditor;
    std::vector<RoasterRow> r;
    r.reserve(roaster.size());
    std::transform(cbegin(roaster),
                   cend(roaster),
                   std::back_inserter(r),
                   [firstDayOfWeek](const auto& elem) {
                       return RoasterRow{
                           elem.first,
                           weekScheduleToString(elem.second, firstDayOfWeek)};
                   });
    view->displayRoaster(r);
}

void displayExceptionalDays(
    std::vector<sprint_timer::WorkSchedule::DateGoal>&& exceptionalDays,
    sprint_timer::ui::contracts::WorkScheduleEditor::View* view)
{
    std::sort(
        begin(exceptionalDays),
        end(exceptionalDays),
        [](const auto& lhs, const auto& rhs) { return lhs.first < rhs.first; });
    view->displayExceptionalDays(exceptionalDays);
}

} // namespace
