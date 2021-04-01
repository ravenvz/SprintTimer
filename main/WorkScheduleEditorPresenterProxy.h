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
#ifndef WORKSCHEDULEEDITORPRESENTERPROXY_H_JRLPYTZH
#define WORKSCHEDULEEDITORPRESENTERPROXY_H_JRLPYTZH

#include <core/CompositionObserver.h>
#include <core/IConfig.h>
#include <core/Observable.h>
#include <qt_gui/presentation/WorkScheduleEditorPresenter.h>

namespace sprint_timer::compose {

class WorkScheduleEditorPresenterProxy
    : public ui::contracts::WorkScheduleEditor::Presenter {
public:
    WorkScheduleEditorPresenterProxy(
        QueryHandler<use_cases::WorkScheduleQuery, WorkSchedule>&
            workScheduleHandler,
        CommandHandler<use_cases::ChangeWorkScheduleCommand>&
            changeWorkScheduleHandler,
        const IConfig& settings,
        Observable& configChangedSignaller);

private:
    QueryHandler<use_cases::WorkScheduleQuery, WorkSchedule>&
        workScheduleHandler;
    CommandHandler<use_cases::ChangeWorkScheduleCommand>&
        changeWorkScheduleHandler;
    const IConfig& settings;
    CompositionObserver configChangedWatcher;
    dw::Weekday cached{settings.firstDayOfWeek()};
    ui::WorkScheduleEditorPresenter presenter{
        workScheduleHandler, changeWorkScheduleHandler, cached};

    void onViewAttached() override;

    void beforeViewDetached() override;

    void onConfigChanged();

    void updateViewImpl() override;

    void onAddExceptionalRequested() override;

    void onExceptionalDaysAdded(dw::Date startDate,
                                uint16_t numDays,
                                uint16_t sprintsPerDay) override;

    void onExceptionalDayRemoved(dw::Date date) override;

    void onWeekScheduleAdded(std::vector<uint8_t>&& schedule,
                             dw::Date startDate) override;

    void onWeekScheduleRemoved(dw::Date scheduleStartDate) override;

    void onScheduleChangeConfirmed() override;

    void onRevertChanges() override;
};

inline WorkScheduleEditorPresenterProxy::WorkScheduleEditorPresenterProxy(
    QueryHandler<use_cases::WorkScheduleQuery, WorkSchedule>&
        workScheduleHandler_,
    CommandHandler<use_cases::ChangeWorkScheduleCommand>&
        changeWorkScheduleHandler_,
    const IConfig& settings_,
    Observable& configChangedSignaller_)
    : workScheduleHandler{workScheduleHandler_}
    , changeWorkScheduleHandler{changeWorkScheduleHandler_}
    , settings{settings_}
    , configChangedWatcher{configChangedSignaller_,
                           [this]() { onConfigChanged(); }}
{
}

inline void WorkScheduleEditorPresenterProxy::onViewAttached()
{
    presenter.attachView(*view().value());
}

inline void WorkScheduleEditorPresenterProxy::beforeViewDetached()
{
    presenter.detachView(*view().value());
}

inline void WorkScheduleEditorPresenterProxy::updateViewImpl()
{
    presenter.updateView();
}

inline void WorkScheduleEditorPresenterProxy::onConfigChanged()
{
    if (settings.firstDayOfWeek() != cached) {
        cached = settings.firstDayOfWeek();
        presenter = ui::WorkScheduleEditorPresenter{
            workScheduleHandler, changeWorkScheduleHandler, cached};
        if (auto v = view(); v) {
            attachView(*view().value());
        }
    }
}

inline void WorkScheduleEditorPresenterProxy::onAddExceptionalRequested()
{
    presenter.onAddExceptionalRequested();
}

inline void WorkScheduleEditorPresenterProxy::onExceptionalDaysAdded(
    dw::Date startDate, uint16_t numDays, uint16_t sprintsPerDay)
{
    presenter.onExceptionalDaysAdded(startDate, numDays, sprintsPerDay);
}

inline void
WorkScheduleEditorPresenterProxy::onExceptionalDayRemoved(dw::Date date)
{
    presenter.onExceptionalDayRemoved(date);
}

inline void WorkScheduleEditorPresenterProxy::onWeekScheduleAdded(
    std::vector<uint8_t>&& schedule, dw::Date startDate)
{
    presenter.onWeekScheduleAdded(std::move(schedule), startDate);
}

inline void WorkScheduleEditorPresenterProxy::onWeekScheduleRemoved(
    dw::Date scheduleStartDate)
{
    presenter.onWeekScheduleRemoved(scheduleStartDate);
}

inline void WorkScheduleEditorPresenterProxy::onScheduleChangeConfirmed()
{
    presenter.onScheduleChangeConfirmed();
}

inline void WorkScheduleEditorPresenterProxy::onRevertChanges()
{
    presenter.onRevertChanges();
}

} // namespace sprint_timer::compose

#endif /* end of include guard: WORKSCHEDULEEDITORPRESENTERPROXY_H_JRLPYTZH */
