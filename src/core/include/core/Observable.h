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
#ifndef OBSERVABLE_H_3WZLFBZN
#define OBSERVABLE_H_3WZLFBZN

#include "core/Observer.h"
#include <unordered_set>

namespace sprint_timer {

class Observable {
public:
    virtual ~Observable() = default;

    virtual void attach(Observer& observer);

    virtual void detach(Observer& observer);

    virtual void notify();

protected:
    std::unordered_set<Observer*> observers;
};

} // namespace sprint_timer

#endif /* end of include guard: OBSERVABLE_H_3WZLFBZN */
