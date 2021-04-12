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
#ifndef COMPOSITEPRESENTER_H_6XHKSCDA
#define COMPOSITEPRESENTER_H_6XHKSCDA

#include <functional>
#include "qt_gui/mvp/AbstractPresenter.h"
#include <vector>

namespace sprint_timer::compose {

class CompositePresenter : public mvp::AbstractPresenter {
public:
    using container_t =
        std::vector<std::reference_wrapper<mvp::AbstractPresenter>>;

    explicit CompositePresenter(container_t&& presenters_)
        : presenters{std::move(presenters_)}
    {
    }

    void updateView() override
    {
        for (auto& element : presenters) {
            element.get().updateView();
        }
    }

private:
    container_t presenters;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: COMPOSITEPRESENTER_H_6XHKSCDA */
