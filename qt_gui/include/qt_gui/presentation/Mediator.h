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
#ifndef MEDIATOR_H_WOD0DXBC
#define MEDIATOR_H_WOD0DXBC

#include <functional>
#include <unordered_set>

namespace sprint_timer::ui {

template <typename ColleagueT> class Mediator {
public:
    virtual ~Mediator() = default;

    void addColleague(ColleagueT* colleague) { colleagues.insert(colleague); }

    void removeColleague(ColleagueT* colleague) { colleagues.erase(colleague); }

    template <typename Functor> void notifyAll(Functor func)
    {
        for (auto* col : colleagues) {
            func(col);
        }
    }

    template <typename Functor> void mediate(ColleagueT* caller, Functor func)
    {
        for (auto* col : colleagues) {
            if (col == caller) {
                continue;
            }
            func(col);
        }
    }

private:
    std::unordered_set<ColleagueT*> colleagues;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: MEDIATOR_H_WOD0DXBC */
