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
#ifndef BASEVIEW_H_KOLQR62J
#define BASEVIEW_H_KOLQR62J

#include "qt_gui/mvp/crtp.h"
#include <optional>

namespace mvp {

template <class ViewImp, class PresenterT>
class BaseView : public crtp<ViewImp> {
public:
    void setPresenter(PresenterT& presenter_)
    {
        maybePresenter = &presenter_;
        maybePresenter->attachView(this->underlying());
    }

    virtual ~BaseView()
    {
        if (maybePresenter != nullptr) {
            maybePresenter->detachView(this->underlying());
        }
    }

    std::optional<PresenterT*> presenter() const
    {
        return (maybePresenter == nullptr) ? std::nullopt
                                           : std::make_optional(maybePresenter);
    }

private:
    BaseView() = default;
    friend ViewImp;

    PresenterT* maybePresenter{nullptr};
};
} // namespace mvp

#endif /* end of include guard: BASEVIEW_H_KOLQR62J */
