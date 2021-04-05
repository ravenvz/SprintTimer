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
#ifndef PROGRESSWIDGET_H_F1ABWAXG
#define PROGRESSWIDGET_H_F1ABWAXG

#include "core/BackRequestStrategy.h"
#include "qt_gui/DistributionRequester.h"
#include "qt_gui/presentation/ProgressPresenterContract.h"
#include "qt_gui/widgets/DisplayableWidget.h"
#include <QWidget>
#include <core/Distribution.h>
#include <core/GoalProgress.h>
#include <core/ProgressComputeStrategy.h>
#include <core/ProgressOverPeriod.h>
#include <core/WorkSchedule.h>
#include <memory>

namespace Ui {
class ProgressWidget;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class ProgressWidget : public QWidget, public contracts::DailyProgress::View {

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

    ProgressWidget(Rows numRows,
                   Columns numColumns,
                   GaugeSize gaugeRelSize,
                   QWidget* parent = nullptr);

    ~ProgressWidget() override;

    void setLegendTitle(const QString& title);

    void setLegendTotalCaption(const QString& caption);

    void setLegendAverageCaption(const QString& caption);

    void setLegendPercentageCaption(const QString& caption);

    void addLegendRow(const QString& labelText, QWidget* field);

    void addLegendRow(QWidget* field);

    void
    displayLegend(const contracts::DailyProgress::LegendData& data) override;

    void displayGauges(const std::vector<contracts::DailyProgress::GaugeValues>&
                           data) override;

    void displayProgressBar(
        const contracts::DailyProgress::ProgressBarData& data) override;

private:
    std::unique_ptr<Ui::ProgressWidget> ui;
    const Rows numRows;
    const Columns numColumns;
    const GaugeSize gaugeRelSize;

    void setupGauges();
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: PROGRESSWIDGET_H_F1ABWAXG */
