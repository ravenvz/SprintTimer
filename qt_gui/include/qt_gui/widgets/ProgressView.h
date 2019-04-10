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
#ifndef PROGRESSVIEW_H_2OXRIURM
#define PROGRESSVIEW_H_2OXRIURM

#include "qt_gui/Synchronizable.h"
#include <QtWidgets/QFrame>
#include <core/Distribution.h>
#include <core/GoalProgress.h>
#include <core/ProgressProto.h>
#include <core/WorkdayTracker.h>
#include <memory>

namespace Ui {
class ProgressView;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class ProgressView : public QFrame, public Synchronizable {
    Q_OBJECT

public:
    virtual ~ProgressView();

    void setLegendTitle(const QString& title);

    void setLegendTotalCaption(const QString& caption);

    void setLegendAverageCaption(const QString& caption);

    void setLegendPercentageCaption(const QString& caption);

    void addLegendRow(const QString& labelText, QWidget* field);

    void addLegendRow(QWidget* field);

    void setData(const ProgressOverPeriod& progress);

    void setWorkingDays(const WorkdayTracker& workdayTracker);

protected:
    using GoalValue = int;
    using Rows = size_t;
    using Columns = size_t;
    using GaugeSize = double;

    ProgressView(Rows numRows,
                 Columns numColumns,
                 GaugeSize gaugeRelSize,
                 QWidget* parent = nullptr);

    WorkdayTracker workdayTracker{utils::WeekdaySelection{0}};

private:
    std::unique_ptr<Ui::ProgressView> ui;
    const Rows numRows;
    const Columns numColumns;
    const GaugeSize gaugeRelSize;

    void setupGauges();

    void updateGauges(const ProgressOverPeriod& progress);

    void updateProgressBar(const GoalProgress& lastBin);

    void updateLegend(const ProgressOverPeriod& progress) const;

signals:
    void goalChanged(int goal);
    void workdaysChange();
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: PROGRESSVIEW_H_2OXRIURM */

