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
#ifndef SHAREDVALUE_H_ETUSINPQ
#define SHAREDVALUE_H_ETUSINPQ

#include <optional>
#include <core/Observable.h>

namespace sprint_timer::ui {

template <typename T>
class SharedValue : public Observable {
public:
    std::optional<T> value() const
    {
        return val;
    }

    void setValue(T value_)
    {
        val = std::move(value_);
    }

private:
    std::optional<T> val;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: SHAREDVALUE_H_ETUSINPQ */
