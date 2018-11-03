/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

#include <QtWidgets/QWidget>
#include <core/Distribution.h>
#include <memory>

namespace Ui {
class BestWorkdayWidget;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {


class BestWorkdayWidget : public QWidget {

public:
    explicit BestWorkdayWidget(QWidget* parent);

    virtual ~BestWorkdayWidget();

    void setData(const Distribution<double>& weekdayDistribution);

private:
    std::unique_ptr<Ui::BestWorkdayWidget> ui;

    void setupWeekdayBarChart();

    void updateWeekdayBarChart(const Distribution<double>& weekdayDistribution);

    void updateWeekdayBarChartLegend(
        const Distribution<double>& weekdayDistribution);
};

} // namespace sprint_timer::ui::qt_gui


#endif // BESTWORKDAYWIDGET_H
