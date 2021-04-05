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
#include "core/CompositionObserver.h"

namespace sprint_timer {

CompositionObserver::CompositionObserver(Observable& configChangedSignaller_,
                                         Callback callback_)
    : configChangedSignaller{configChangedSignaller_}
    , callback{callback_}
{
    configChangedSignaller.attach(*this);
}

CompositionObserver::~CompositionObserver()
{
    configChangedSignaller.detach(*this);
}

void CompositionObserver::update() { callback(); }

} // namespace sprint_timer
