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
#include "qt_gui/widgets/ProgressWidget.h"
#include "qt_gui/widgets/Gauge.h"
#include "ui_progress_widget.h"
#include <QtWidgets/QGridLayout>

namespace sprint_timer::ui::qt_gui {

ProgressWidget::ProgressWidget(Rows numRows_,
                               Columns numColumns_,
                               GaugeSize gaugeRelSize_,
                               QWidget* parent)
    : QWidget{parent}
    , ui{std::make_unique<Ui::ProgressWidget>()}
    , numRows{numRows_}
    , numColumns{numColumns_}
    , gaugeRelSize{gaugeRelSize_}
{
    ui->setupUi(this);
    setupGauges();
}

ProgressWidget::~ProgressWidget() = default;

void ProgressWidget::setupGauges()
{
    for (size_t row = 0; row < numRows.value; ++row) {
        for (size_t col = 0; col < numColumns.value; ++col) {
            auto gauge =
                std::make_unique<Gauge>(0, 0, gaugeRelSize.value, this);
            ui->gaugeLayout->addWidget(
                gauge.release(), static_cast<int>(row), static_cast<int>(col));
        }
    }
}

void ProgressWidget::setLegendTitle(const QString& title)
{
    ui->lblTitle->setText(title);
}

void ProgressWidget::setLegendTotalCaption(const QString& caption)
{
    ui->lblTotalCaption->setText(caption);
}

void ProgressWidget::setLegendAverageCaption(const QString& caption)
{
    ui->lblAverageCaption->setText(caption);
}

void ProgressWidget::setLegendPercentageCaption(const QString& caption)
{
    ui->lblProgressPercentageCaption->setText(caption);
}

void ProgressWidget::addLegendRow(const QString& labelText, QWidget* field)
{
    ui->formLayout->addRow(labelText, field);
}

void ProgressWidget::addLegendRow(QWidget* field)
{
    ui->formLayout->addRow(field);
}

void ProgressWidget::displayLegend(
    const contracts::DailyProgress::LegendData& legendData)
{
    ui->lblProgress->setText(QString::fromStdString(legendData.count));
    ui->lblLeftCaption->setText(QString::fromStdString(legendData.left));
    ui->lblLeft->setText(QString::fromStdString(legendData.difference));
    ui->lblAverage->setText(QString::fromStdString(legendData.average));
    ui->lblPercentage->setText(QString::fromStdString(legendData.percentage));
}

void ProgressWidget::displayGauges(
    const std::vector<contracts::DailyProgress::GaugeValues>& values)
{
    for (size_t row = 0, ind = 0; row < numRows.value; ++row) {
        for (size_t col = 0; col < numColumns.value; ++col, ++ind) {
            QLayoutItem* item = ui->gaugeLayout->itemAtPosition(
                static_cast<int>(row), static_cast<int>(col));
            if (item == nullptr) {
                return;
            }
            const auto& [estimated, actual, emptyColor, filledColor] =
                values[ind];
            dynamic_cast<Gauge*>(item->widget())
                ->setData(actual, estimated, emptyColor, filledColor);
        }
    }
}

void ProgressWidget::displayProgressBar(
    const contracts::DailyProgress::ProgressBarData& barData)
{
    QPalette palette = ui->progressBarGoalProgress->palette();
    palette.setColor(QPalette::Highlight,
                     QColor{QString::fromStdString(barData.filledColor)});
    palette.setColor(QPalette::Base,
                     QColor{QString::fromStdString(barData.emptyColor)});
    ui->progressBarGoalProgress->setPalette(palette);
    ui->progressBarGoalProgress->setMaximum(barData.estimated);
    ui->progressBarGoalProgress->setFormat(QString{"%1"}.arg(barData.actual) +
                                           QString{"/%m"});
    if (barData.estimated && barData.actual != barData.estimated)
        ui->progressBarGoalProgress->setValue(barData.actual %
                                              barData.estimated);
    else
        ui->progressBarGoalProgress->setValue(barData.actual);
    ui->progressBarGoalProgress->setVisible(barData.isVisible);
    ui->progressBarGoalProgress->repaint();
}

} // namespace sprint_timer::ui::qt_gui
