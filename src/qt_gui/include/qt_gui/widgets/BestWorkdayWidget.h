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
#ifndef BESTWORKDAYWIDGET_H
#define BESTWORKDAYWIDGET_H

#include "qt_gui/presentation/BestWorkdayContract.h"
#include "qt_gui/widgets/BarChart.h"
#include <QtWidgets/QWidget>
#include "core/Distribution.h"
#include <memory>

class QLabel;

namespace sprint_timer::ui::qt_gui {

class BestWorkdayWidget : public QWidget, public contracts::BestWorkday::View {
public:
    explicit BestWorkdayWidget(QWidget* parent = nullptr);

private:
    QLabel* labelBestWorkdayName;
    QLabel* labelBestWorkdayMsg;
    BarChart* workdayBarChart;
    // std::unique_ptr<Ui::BestWorkdayWidget> ui;

    void displayBars(const contracts::BestWorkday::View::BarD& data) override;

    void displayLegend(
        const contracts::BestWorkday::View::LegendData& data) override;
};

} // namespace sprint_timer::ui::qt_gui

#endif // BESTWORKDAYWIDGET_H
