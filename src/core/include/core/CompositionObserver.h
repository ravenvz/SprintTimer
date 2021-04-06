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
#ifndef COMPOSITIONOBSERVER_H_ACUA0FSO
#define COMPOSITIONOBSERVER_H_ACUA0FSO

#include "core/Observable.h"
#include "core/Observer.h"
#include <functional>

namespace sprint_timer {

/* Utility class suitable to be composed inside other type that doesn't want to
 * extend Observable. */
class CompositionObserver : public Observer {
public:
    using Callback = std::function<void()>;

    CompositionObserver(Observable& configChangedSignaller_,
                        Callback callback_);

    ~CompositionObserver() override;

    void update() override;

private:
    Observable& configChangedSignaller;
    Callback callback;
};

} // namespace sprint_timer

#endif /* end of include guard: COMPOSITIONOBSERVER_H_ACUA0FSO */
