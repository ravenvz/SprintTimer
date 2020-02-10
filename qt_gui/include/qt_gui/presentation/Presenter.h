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
#ifndef BASEPRESENTER_H_9G4DRMSO
#define BASEPRESENTER_H_9G4DRMSO

namespace sprint_timer::ui {

template <typename ViewT> class Presenter {
public:
    virtual ~Presenter() = default;

    virtual void attachView(ViewT& view_)
    {
        view = &view_;
        onViewAttached();
    }

    virtual void onViewAttached() = 0;

protected:
    ViewT* view{nullptr};
};

} // namespace sprint_timer::ui

#endif /* end of include guard: BASEPRESENTER_H_9G4DRMSO */
