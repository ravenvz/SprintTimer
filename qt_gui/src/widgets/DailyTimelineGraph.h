/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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

#ifndef DAILYTIMELINEGRAPH_H
#define DAILYTIMELINEGRAPH_H


#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <date_wrapper/DateTime.h>
#include <core/Distribution.h>

namespace Ui {
class DailyTimelineGraph;
} // namespace Ui


class DailyTimelineGraph : public QFrame {
public:
    explicit DailyTimelineGraph(QWidget* parent);

    ~DailyTimelineGraph();

    void setData(const Distribution<double>& dailyDistribution,
                 const QDate& startDate,
                 int dailyGoal);

private:
    Ui::DailyTimelineGraph* ui;

    void setupGraphs();

    void updateLegend(const Distribution<double>& dailyDistribution);
};


#endif // DAILYTIMELINEGRAPH_H

