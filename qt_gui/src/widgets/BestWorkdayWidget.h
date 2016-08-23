/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#ifndef BESTWORKDAYWIDGET_H
#define BESTWORKDAYWIDGET_H


#include <QtWidgets/QWidget>
#include <core/Distribution.h>

namespace Ui {
class BestWorkdayWidget;
}


class BestWorkdayWidget : public QWidget {

public:
    explicit BestWorkdayWidget(QWidget* parent);

    virtual ~BestWorkdayWidget();

    void setData(const Distribution<double>& weekdayDistribution);

private:
    Ui::BestWorkdayWidget* ui;

    void setupWeekdayBarChart();

    void updateWeekdayBarChart(const Distribution<double>& weekdayDistribution);

    void updateWeekdayBarChartLegend(
        const Distribution<double>& weekdayDistribution);
};


#endif // BESTWORKDAYWIDGET_H
