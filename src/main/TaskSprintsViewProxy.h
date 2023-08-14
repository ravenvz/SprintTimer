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
#ifndef TASKSPRINTSVIEWPROXY_H_XBJVYFVL
#define TASKSPRINTSVIEWPROXY_H_XBJVYFVL

#include "DisplayableLifestyleProxy.h"
#include "qt_gui/widgets/TaskSprintsView.h"

namespace sprint_timer::compose {

class TaskSprintsViewProxy : public DisplayableLifestyleProxy {
public:
    TaskSprintsViewProxy(
        mvp::BasePresenter<ui::contracts::TaskSprintsContract::View>&
            presenter_,
        QStyledItemDelegate& delegate,
        const patterns::Converter<QDate, dw::Date>& dateConverter);

private:
    mvp::BasePresenter<ui::contracts::TaskSprintsContract::View>& presenter;
    QStyledItemDelegate& delegate;
    const patterns::Converter<QDate, dw::Date>& dateConverter;
    std::unique_ptr<ui::qt_gui::StandaloneDisplayableWidget> create() override;
};

TaskSprintsViewProxy::TaskSprintsViewProxy(
    mvp::BasePresenter<ui::contracts::TaskSprintsContract::View>& presenter_,
    QStyledItemDelegate& delegate_,
    const patterns::Converter<QDate, dw::Date>& dateConverter_)
    : presenter{presenter_}
    , delegate{delegate_}
    , dateConverter{dateConverter_}
{
}

inline std::unique_ptr<ui::qt_gui::StandaloneDisplayableWidget>
TaskSprintsViewProxy::create()
{
    auto taskSprintsView =
        std::make_unique<ui::qt_gui::TaskSprintsView>(delegate, dateConverter);
    presenter.attachView(*taskSprintsView);
    return taskSprintsView;
}

} // namespace sprint_timer::compose

#endif /* end of include guard: TASKSPRINTSVIEWPROXY_H_XBJVYFVL */
