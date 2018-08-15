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
#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QtWidgets/QFrame>
#include <core/Distribution.h>
#include <memory>

namespace Ui {
class ProgressView;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class ProgressView : public QFrame {
    Q_OBJECT

public:
    using GoalValue = int;
    using Rows = size_t;
    using Columns = size_t;
    using GaugeSize = double;

    ProgressView(GoalValue goal,
                 Rows numRows,
                 Columns numColumns,
                 GaugeSize gaugeRelSize,
                 QWidget* parent = nullptr);

    ~ProgressView();

    void setLegendTitle(const QString& title);

    void setLegendTotalCaption(const QString& caption);

    void setLegendAverageCaption(const QString& caption);

    void setLegendPercentageCaption(const QString& caption);

    void setLegendGoalCaption(const QString& caption);

    void addLegendRow(const QString& labelText, QWidget* field);

    void setData(const Distribution<int>& distribution, size_t numActiveBins);

private:
    std::unique_ptr<Ui::ProgressView> ui;
    GoalValue goal;
    const Rows numRows;
    const Columns numColumns;
    const GaugeSize gaugeRelSize;

    void setupGauges();

    void fillGauges(const Distribution<int>& distribution);

    void updateProgressBar(int lastValue);

signals:
    void goalChanged(int goal);
};

} // namespace sprint_timer::ui::qt_gui


#endif // PROGRESSWIDGET_H
