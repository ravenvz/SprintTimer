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

#include "qt_gui/BackRequestStrategy.h"
#include "qt_gui/DistributionRequester.h"
#include "qt_gui/WorkScheduleWrapper.h"
#include <QtWidgets/QFrame>
#include <core/Distribution.h>
#include <core/GoalProgress.h>
#include <core/GroupByPeriodStrategy.h>
#include <core/ProgressOverPeriod.h>
#include <core/WorkSchedule.h>
#include <memory>

namespace Ui {
class ProgressView;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class ProgressView : public QFrame {

public:
    using GoalValue = int;

    struct Rows {
        size_t value{0};
    };

    struct Columns {
        size_t value{0};
    };

    struct GaugeSize {
        double value{0};
    };

    ProgressView(const DistributionRequester& distributionRequester,
                 const WorkScheduleWrapper& workScheduleWrapper,
                 const GroupByPeriodStrategy& groupingStrategy,
                 const BackRequestStrategy& backRequestStrategy,
                 Rows numRows,
                 Columns numColumns,
                 GaugeSize gaugeRelSize,
                 QWidget* parent = nullptr);

    virtual ~ProgressView();

    void setLegendTitle(const QString& title);

    void setLegendTotalCaption(const QString& caption);

    void setLegendAverageCaption(const QString& caption);

    void setLegendPercentageCaption(const QString& caption);

    void addLegendRow(const QString& labelText, QWidget* field);

    void addLegendRow(QWidget* field);

    void setData(const ProgressOverPeriod& progress);

private:
    std::unique_ptr<Ui::ProgressView> ui;
    const Rows numRows;
    const Columns numColumns;
    const GaugeSize gaugeRelSize;
    QMetaObject::Connection distributionConnection;
    QMetaObject::Connection workScheduleConnection;

    void setupGauges();

    void updateGauges(const ProgressOverPeriod& progress);

    void updateProgressBar(const GoalProgress& lastBin);

    void updateLegend(const ProgressOverPeriod& progress) const;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: PROGRESSVIEW_H_2OXRIURM */

