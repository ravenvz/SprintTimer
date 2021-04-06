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
#ifndef BASEPRESENTER_H_JFOEV97B
#define BASEPRESENTER_H_JFOEV97B

#include "qt_gui/mvp/AbstractPresenter.h"
#include <iostream>
#include <optional>

namespace mvp {

template <typename ViewT> class BasePresenter : public AbstractPresenter {
public:
    void updateView() override
    {
        if (maybeView) {
            updateViewImpl();
            return;
        }
        std::cout << "No view is attached; updating aborted" << std::endl;
    }

    void attachView(ViewT& view)
    {
        beforeViewAttached();
        maybeView = &view;
        onViewAttached();
    }

    void detachView(ViewT& /* view */)
    {
        beforeViewDetached();
        maybeView = nullptr;
        onViewDetached();
    }

    std::optional<ViewT*> view() const
    {
        return (maybeView == nullptr) ? std::nullopt
                                      : std::make_optional(maybeView);
    }

private:
    ViewT* maybeView{nullptr};

    virtual void updateViewImpl() { }

    virtual void beforeViewAttached() { }

    virtual void onViewAttached() { }

    virtual void beforeViewDetached() { }

    virtual void onViewDetached() { }
};
} // namespace mvp

#endif /* end of include guard: BASEPRESENTER_H_JFOEV97B */
