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
#ifndef PROGRESSVIEWPROXY_H_JPYHUVZI
#define PROGRESSVIEWPROXY_H_JPYHUVZI

#include "ManagedDisplayable.h"
#include <qt_gui/presentation/Presenter.h>
#include <qt_gui/widgets/ProgressWidget.h>

namespace sprint_timer::compose {

class ProgressViewProxy : public ManagedDisplayable {
public:
    ProgressViewProxy(
        ui::Presenter<ui::contracts::DailyProgress::View>& presenter,
        ui::qt_gui::ProgressWidget::Rows rows,
        ui::qt_gui::ProgressWidget::Columns columns,
        ui::qt_gui::ProgressWidget::GaugeSize gaugeRelSize)
        : presenter{presenter}
        , rows{rows}
        , columns{columns}
        , gaugeRelSize{gaugeRelSize}
    {
    }

    std::unique_ptr<Displayable> create() override
    {
        return std::make_unique<ui::qt_gui::ProgressWidget>(
            presenter, rows, columns, gaugeRelSize);
    }

private:
    ui::Presenter<ui::contracts::DailyProgress::View>& presenter;
    ui::qt_gui::ProgressWidget::Rows rows;
    ui::qt_gui::ProgressWidget::Columns columns;
    ui::qt_gui::ProgressWidget::GaugeSize gaugeRelSize;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: PROGRESSVIEWPROXY_H_JPYHUVZI */
