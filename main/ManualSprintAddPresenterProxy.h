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
#ifndef MANUALSPRINTADDPRESENTERPROXY_H_BRJGVTAG
#define MANUALSPRINTADDPRESENTERPROXY_H_BRJGVTAG

#include <core/CompositionObserver.h>
#include <core/IConfig.h>
#include <qt_gui/presentation/ManualSprintAddPresenter.h>

namespace sprint_timer::compose {

class ManualSprintAddPresenterProxy
    : public ui::contracts::ManualSprintAddContract::Presenter {
public:
    ManualSprintAddPresenterProxy(
        CommandHandler<use_cases::RegisterSprintBulkCommand>&
            registerSprintsHandler,
        QueryHandler<use_cases::UnfinishedTasksQuery,
                     std::vector<entities::Task>>& activeTasksHandler,
        const IConfig& settings,
        Observable& configChangedSignaller);

    void
    attachView(ui::contracts::ManualSprintAddContract::View& view_) override;

    void
    detachView(ui::contracts::ManualSprintAddContract::View& view_) override;

    void onSprintAddRequested() override;

    void onSprintAddConfirmed(size_t taskNumber,
                              dw::DateTime firstSprintStart,
                              size_t numSprints) override;

private:
    struct CachedSettings {
        dw::Weekday firstDayOfWeek;
        std::chrono::seconds sprintDuration;
    };

    CommandHandler<use_cases::RegisterSprintBulkCommand>&
        registerSprintsHandler;
    QueryHandler<use_cases::UnfinishedTasksQuery, std::vector<entities::Task>>&
        activeTasksHandler;
    const IConfig& settings;
    ui::ManualSprintAddPresenter presenter{registerSprintsHandler,
                                           activeTasksHandler,
                                           settings.firstDayOfWeek(),
                                           settings.sprintDuration()};
    CompositionObserver observer;
    CachedSettings cached{settings.firstDayOfWeek(), settings.sprintDuration()};

    void onConfigChanged();

    void updateViewImpl() override { }
};

inline ManualSprintAddPresenterProxy::ManualSprintAddPresenterProxy(
    CommandHandler<use_cases::RegisterSprintBulkCommand>&
        registerSprintsHandler_,
    QueryHandler<use_cases::UnfinishedTasksQuery, std::vector<entities::Task>>&
        activeTasksHandler_,
    const IConfig& settings_,
    Observable& configChangedSignaller_)
    : registerSprintsHandler{registerSprintsHandler_}
    , activeTasksHandler{activeTasksHandler_}
    , settings{settings_}
    , observer{configChangedSignaller_, [this]() { onConfigChanged(); }}
{
}

inline void ManualSprintAddPresenterProxy::attachView(
    ui::contracts::ManualSprintAddContract::View& view_)
{
    presenter.attachView(view_);
    BasePresenter::attachView(view_);
}

inline void ManualSprintAddPresenterProxy::detachView(
    ui::contracts::ManualSprintAddContract::View& view_)
{
    presenter.detachView(view_);
    BasePresenter::detachView(view_);
}

inline void ManualSprintAddPresenterProxy::onSprintAddRequested()
{
    presenter.onSprintAddRequested();
}

inline void ManualSprintAddPresenterProxy::onSprintAddConfirmed(
    size_t taskNumber, dw::DateTime firstSprintStart, size_t numSprints)
{
    presenter.onSprintAddConfirmed(taskNumber, firstSprintStart, numSprints);
}

inline void ManualSprintAddPresenterProxy::onConfigChanged()
{
    const auto firstDay = settings.firstDayOfWeek();
    const auto duration = settings.sprintDuration();

    if (cached.firstDayOfWeek == firstDay && cached.sprintDuration == duration)
        return;

    cached.firstDayOfWeek = firstDay;
    cached.sprintDuration = duration;
    presenter = ui::ManualSprintAddPresenter{
        registerSprintsHandler, activeTasksHandler, firstDay, duration};
    presenter.attachView(*view);
}

} // namespace sprint_timer::compose

#endif /* end of include guard: MANUALSPRINTADDPRESENTERPROXY_H_BRJGVTAG */
