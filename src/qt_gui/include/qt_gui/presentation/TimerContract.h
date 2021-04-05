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
#ifndef TIMERCONTRACT_H_DSHEHJTD
#define TIMERCONTRACT_H_DSHEHJTD

#include "qt_gui/mvp/BasePresenter.h"
#include "qt_gui/mvp/BaseView.h"
#include "qt_gui/presentation/TaskSelectionMediator.h"
#include <chrono>
#include <date_wrapper/date_wrapper.h>
#include <optional>
#include <string_view>
#include <vector>

namespace sprint_timer::ui::contracts::TimerContract {

struct TimerUiModel;

class View;

class Presenter : public mvp::BasePresenter<View>,
                  public TaskSelectionColleague {
public:
    virtual void onTimerClicked() = 0;

    virtual void onCancelClicked() = 0;

    virtual void onZoneClicked() = 0;

    virtual void changeTaskSelection(size_t index, std::string&& uuid) = 0;
};

class View : public mvp::BaseView<View, Presenter> {
public:
    virtual void updateTimerValue(std::chrono::seconds timerValue) = 0;

    virtual void setupUi(TimerUiModel&& model) = 0;

    virtual void
    submitSprints(const std::vector<dw::DateTimeRange>& timeIntervals) = 0;

    virtual void selectTask(size_t taskIndex) = 0;
};

struct TimerUiModel {
    bool submissionBoxVisible;
    bool cancelControlVisible;
    bool zoneControlVisible;
    bool dialVisible;
    bool invertedDial;
    std::optional<std::string_view> dialColor;
    std::optional<std::string_view> centerText;
    std::optional<std::chrono::seconds> timerTotalDuration;

    [[nodiscard]] static TimerUiModel idleUiModel(std::string_view text_)
    {
        return TimerUiModel{false,
                            false,
                            false,
                            false,
                            false,
                            std::nullopt,
                            std::optional<std::string_view>{text_},
                            std::nullopt};
    }

    [[nodiscard]] static TimerUiModel
    runningUiModel(std::chrono::seconds timerTotalDuration_,
                   std::string_view dialColor_,
                   bool zoneControlVisible_)
    {
        return TimerUiModel{false,
                            true,
                            zoneControlVisible_,
                            true,
                            false,
                            std::optional<std::string_view>{dialColor_},
                            std::nullopt,
                            timerTotalDuration_};
    }

    [[nodiscard]] static TimerUiModel
    returnFromZoneUiModel(std::string_view dialColor_)
    {
        return TimerUiModel{false,
                            true,
                            true,
                            true,
                            false,
                            std::optional<std::string_view>{dialColor_},
                            std::nullopt,
                            std::nullopt};
    }

    [[nodiscard]] static TimerUiModel
    sprintFinishedUiModel(std::string_view centerText_)
    {
        return TimerUiModel{true,
                            true,
                            false,
                            false,
                            false,
                            std::nullopt,
                            std::optional<std::string_view>{centerText_},
                            std::nullopt};
    }

    [[nodiscard]] static TimerUiModel
    zoneModeUiModel(std::string_view dialColor_)
    {
        return TimerUiModel{false,
                            false,
                            true,
                            true,
                            false,
                            std::optional<std::string_view>{dialColor_},
                            std::nullopt,
                            std::nullopt};
    }

private:
    TimerUiModel(bool submissionBoxVisible_,
                 bool cancelControlVisible_,
                 bool zoneControlVisible_,
                 bool dialVisible_,
                 bool invertedDial_,
                 std::optional<std::string_view> dialColor_,
                 std::optional<std::string_view> centerText_,
                 std::optional<std::chrono::seconds> timerTotalDuration_)
        : submissionBoxVisible{submissionBoxVisible_}
        , cancelControlVisible{cancelControlVisible_}
        , zoneControlVisible{zoneControlVisible_}
        , dialVisible{dialVisible_}
        , invertedDial{invertedDial_}
        , dialColor{dialColor_}
        , centerText{centerText_}
        , timerTotalDuration{timerTotalDuration_}
    {
    }
};

} // namespace sprint_timer::ui::contracts::TimerContract

#endif /* end of include guard: TIMERCONTRACT_H_DSHEHJTD */
