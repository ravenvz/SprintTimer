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
#ifndef BESTWORKDAYWIDGET_H
#define BESTWORKDAYWIDGET_H

#include "qt_gui/presentation/BasePresenter.h"
#include "qt_gui/presentation/BestWorkdayContract.h"
#include <QtWidgets/QWidget>
#include <core/Distribution.h>
#include <core/SprintStatistics.h>
#include <memory>

namespace Ui {
class BestWorkdayWidget;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class BestWorkdayWidget : public QWidget, public contracts::BestWorkday::View {

public:
    explicit BestWorkdayWidget(
        BasePresenter<contracts::BestWorkday::View>& presenter,
        QWidget* parent = nullptr);

    virtual ~BestWorkdayWidget() override;

private:
    std::unique_ptr<Ui::BestWorkdayWidget> ui;
    BasePresenter<contracts::BestWorkday::View>& presenter;

    void displayBars(const contracts::BestWorkday::View::BarD& data) override;

    void displayLegend(
        const contracts::BestWorkday::View::LegendData& data) override;
};

} // namespace sprint_timer::ui::qt_gui

#endif // BESTWORKDAYWIDGET_H
