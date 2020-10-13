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
#ifndef WORKFLOWPROXY_H_VT728IZA
#define WORKFLOWPROXY_H_VT728IZA

#include <core/CompositionObserver.h>
#include <core/IConfig.h>
#include <core/Workflow.h>

namespace sprint_timer::compose {

class WorkflowProxy : public IWorkflow {
public:
    WorkflowProxy(std::chrono::seconds timerTickPeriod,
                  const IConfig& settings,
                  Observable& configChangedSignaller);

    void start() override;

    void cancel() override;

    std::chrono::seconds currentDuration() const override;

    void toggleInTheZoneMode() override;

    std::vector<dw::DateTimeRange> completedSprints() const override;

    void setNumFinishedSprints(int num) override;

    void addListener(WorkflowListener* listener) override;

    void removeListener(WorkflowListener* listener) override;

    void onConfigChanged();

    void reconfigure(const IWorkflow::WorkflowParams& params) override;

private:
    std::chrono::seconds timerTickPeriod;
    const IConfig& settings;
    CompositionObserver configChangedWatcher;
    IWorkflow::WorkflowParams workflowParams{settings.sprintDuration(),
                                             settings.shortBreakDuration(),
                                             settings.longBreakDuration(),
                                             settings.numSprintsBeforeBreak()};
    Workflow wrapped{timerTickPeriod, workflowParams};

    bool interestingConfigParamsUnchanged();
};

inline WorkflowProxy::WorkflowProxy(std::chrono::seconds timerTickPeriod_,
                                    const IConfig& settings_,
                                    Observable& configChangedSignaller_)
    : timerTickPeriod{timerTickPeriod_}
    , settings{settings_}
    , configChangedWatcher{configChangedSignaller_,
                           [this]() { onConfigChanged(); }}
{
}

inline void WorkflowProxy::start() { wrapped.start(); }

inline void WorkflowProxy::cancel() { wrapped.cancel(); }

inline std::chrono::seconds WorkflowProxy::currentDuration() const
{
    return wrapped.currentDuration();
}

inline void WorkflowProxy::toggleInTheZoneMode()
{
    wrapped.toggleInTheZoneMode();
}

inline std::vector<dw::DateTimeRange> WorkflowProxy::completedSprints() const
{
    return wrapped.completedSprints();
}

inline void WorkflowProxy::setNumFinishedSprints(int num)
{
    wrapped.setNumFinishedSprints(num);
}

inline void WorkflowProxy::addListener(WorkflowListener* listener)
{
    wrapped.addListener(listener);
}

inline void WorkflowProxy::removeListener(WorkflowListener* listener)
{
    wrapped.removeListener(listener);
}

inline void WorkflowProxy::onConfigChanged()
{
    wrapped.reconfigure(
        IWorkflow::WorkflowParams{settings.sprintDuration(),
                                  settings.shortBreakDuration(),
                                  settings.longBreakDuration(),
                                  settings.numSprintsBeforeBreak()});
}

inline void WorkflowProxy::reconfigure(const IWorkflow::WorkflowParams& params)
{
    wrapped.reconfigure(params);
}

} // namespace sprint_timer::compose

#endif /* end of include guard: WORKFLOWPROXY_H_VT728IZA */

