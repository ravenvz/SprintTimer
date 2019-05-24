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
#include "qt_gui/widgets/ProgressView.h"
#include "qt_gui/widgets/Gauge.h"
#include "ui_progress_widget.h"
#include <QtWidgets/QGridLayout>

#include <iostream>

namespace ProgressBarColors {

const QColor targetGoalReached = QColor("#6baa15");
const QColor overwork = Qt::red;
const QColor workInProgress = Qt::gray;

} // namespace ProgressBarColors

namespace {

QString formatDecimal(double decimal)
{
    return QString("%1").arg(decimal, 2, 'f', 2, '0');
}

} // namespace

namespace sprint_timer::ui::qt_gui {

ProgressView::ProgressView(
    const DistributionModel& progressModel_,
    const WorkdayTrackerModel& workdaysModel_,
    const ProgressGroupingStrategy& groupingStrategy_,
    const ProgressRangeRequestStrategy& requestRangeStrategy_,
    Rows numRows_,
    Columns numColumns_,
    GaugeSize gaugeRelSize_,
    QWidget* parent)
    : QFrame{parent}
    , ui{std::make_unique<Ui::ProgressView>()}
    , numRows{numRows_}
    , numColumns{numColumns_}
    , gaugeRelSize{gaugeRelSize_}
{
    ui->setupUi(this);
    setupGauges();
    updateProgressBar(GoalProgress{0, 0});
    connect(&progressModel_,
            &DistributionModel::distributionChanged,
            [&](const std::vector<int>& updatedDistribution) {
                const ProgressOverPeriod progress{
                    requestRangeStrategy_.dateRange(),
                    updatedDistribution,
                    workdaysModel_.workdayTracker(),
                    groupingStrategy_};
                setData(progress);
            });
    connect(&workdaysModel_,
            &WorkdayTrackerModel::workdaysChanged,
            [&](const WorkdayTracker& updatedTracker) {
                ProgressOverPeriod progress{requestRangeStrategy_.dateRange(),
                                            progressModel_.distribution(),
                                            updatedTracker,
                                            groupingStrategy_};
                setData(progress);
            });
}

void ProgressView::setupGauges()
{
    for (size_t row = 0; row < numRows; ++row) {
        for (size_t col = 0; col < numColumns; ++col) {
            auto gauge = std::make_unique<Gauge>(0, 0, gaugeRelSize, this);
            ui->gaugeLayout->addWidget(
                gauge.release(), static_cast<int>(row), static_cast<int>(col));
        }
    }
}

ProgressView::~ProgressView() = default;

void ProgressView::setLegendTitle(const QString& title)
{
    ui->lblTitle->setText(title);
}

void ProgressView::setLegendTotalCaption(const QString& caption)
{
    ui->lblTotalCaption->setText(caption);
}

void ProgressView::setLegendAverageCaption(const QString& caption)
{
    ui->lblAverageCaption->setText(caption);
}

void ProgressView::setLegendPercentageCaption(const QString& caption)
{
    ui->lblProgressPercentageCaption->setText(caption);
}

void ProgressView::addLegendRow(const QString& labelText, QWidget* field)
{
    ui->formLayout->addRow(labelText, field);
}

void ProgressView::addLegendRow(QWidget* field)
{
    ui->formLayout->addRow(field);
}

void ProgressView::setData(const ProgressOverPeriod& progress)
{
    updateLegend(progress);
    updateGauges(progress);
    updateProgressBar(progress.getValue(progress.size() - 1));
}

void ProgressView::updateLegend(const ProgressOverPeriod& progress) const
{
    ui->lblProgress->setText(
        QString("%1/%2").arg(progress.actual()).arg(progress.estimated()));
    ui->lblLeftCaption->setText(progress.isOverwork() ? "Overwork:"
                                                      : "Left to complete:");
    ui->lblLeft->setText(QString{"%1"}.arg(abs(progress.difference())));
    if (auto average = progress.averagePerGroupPeriod(); average)
        ui->lblAverage->setText(formatDecimal(*average));
    else
        ui->lblAverage->setText("n/a");
    if (auto percentage = progress.percentage(); percentage)
        ui->lblPercentage->setText(
            QString{"%1%"}.arg(formatDecimal(*progress.percentage())));
    else
        ui->lblPercentage->setText(QString{"n/a"});
}

void ProgressView::updateGauges(const ProgressOverPeriod& progress)
{
    for (size_t row = 0, ind = 0; row < numRows; ++row) {
        for (size_t col = 0; col < numColumns; ++col, ++ind) {
            QLayoutItem* item = ui->gaugeLayout->itemAtPosition(
                static_cast<int>(row), static_cast<int>(col));
            if (item)
                dynamic_cast<Gauge*>(item->widget())
                    ->setData(progress.getValue(ind));
        }
    }
}

void ProgressView::updateProgressBar(const GoalProgress& lastBin)
{
    auto bar = ui->progressBarGoalProgress;
    bar->setMaximum(lastBin.estimated());
    bar->setFormat("%v/%m");
    bar->hide();
    if (lastBin.estimated() == 0) {
        return;
    }
    QPalette p = bar->palette();
    // TODO maybe it's time to define it's own type for progress bar?
    int actual = lastBin.actual();
    // TODO replace with lastBin.isOverwork() etc.
    if (lastBin.actual() == lastBin.estimated()) {
        p.setColor(QPalette::Highlight, ProgressBarColors::targetGoalReached);
        p.setColor(QPalette::Base, ProgressBarColors::targetGoalReached);
    }
    else if (lastBin.actual() > lastBin.estimated()) {
        p.setColor(QPalette::Highlight, ProgressBarColors::overwork);
        p.setColor(QPalette::Base, ProgressBarColors::targetGoalReached);
        bar->setFormat(QString("%1").arg(actual) + QString("/%m"));
        actual %= lastBin.estimated();
    }
    else {
        p.setColor(QPalette::Highlight, ProgressBarColors::workInProgress);
        p.setColor(QPalette::Base, Qt::white);
    }
    bar->setPalette(p);
    bar->setValue(actual);
    bar->show();
}


} // namespace sprint_timer::ui::qt_gui
