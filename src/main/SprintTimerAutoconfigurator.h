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
#ifndef SPRINTTIMERAUTOCONFIGURATOR_H_X2FSODL8
#define SPRINTTIMERAUTOCONFIGURATOR_H_X2FSODL8


#include "api/IConfig.h"
#include "core/CompositionObserver.h"
#include "core/SprintTimer.h"

namespace sprint_timer::compose {

class SprintTimerAutoconfigurator {
public:
    SprintTimerAutoconfigurator(SprintTimer& subject,
                                const api::IConfig& settings,
                                Observable& configChangedSignaller_);

private:
    SprintTimer& subject;
    const api::IConfig& settings;
    CompositionObserver configChangedWatcher;

    auto onConfigChanged()
    {
        subject.reconfigure(
            SprintTimer::WorkflowParams{settings.sprintDuration(),
                                          settings.shortBreakDuration(),
                                          settings.longBreakDuration(),
                                          settings.numSprintsBeforeBreak()});
    }
};

inline SprintTimerAutoconfigurator::SprintTimerAutoconfigurator(
    SprintTimer& subject_,
    const api::IConfig& settings_,
    Observable& configChangedSignaller_)
    : subject{subject_}
    , settings{settings_}
    , configChangedWatcher{configChangedSignaller_,
                           [this]() { onConfigChanged(); }}
{
}

} // namespace sprint_timer::compose


#endif /* end of include guard: SPRINTTIMERAUTOCONFIGURATOR_H_X2FSODL8 */


