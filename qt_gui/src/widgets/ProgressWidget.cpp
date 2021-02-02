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
#include "qt_gui/widgets/ProgressWidget.h"
#include "qt_gui/widgets/Gauge.h"
#include "ui_progress_widget.h"
#include <QtWidgets/QGridLayout>

namespace sprint_timer::ui::qt_gui {

ProgressWidget::ProgressWidget(
    BasePresenter<contracts::DailyProgress::View>& presenter_,
    Rows numRows_,
    Columns numColumns_,
    GaugeSize gaugeRelSize_,
    QWidget* parent)
    : QWidget{parent}
    , ui{std::make_unique<Ui::ProgressWidget>()}
    , presenter{presenter_}
    , numRows{numRows_}
    , numColumns{numColumns_}
    , gaugeRelSize{gaugeRelSize_}
{
    ui->setupUi(this);
    setupGauges();
    presenter.attachView(*this);
}

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

ProgressWidget::~ProgressWidget() { presenter.detachView(*this); }

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
    const contracts::DailyProgress::LegendData& data)
{
    ui->lblProgress->setText(QString::fromStdString(data.count));
    ui->lblLeftCaption->setText(QString::fromStdString(data.left));
    ui->lblLeft->setText(QString::fromStdString(data.difference));
    ui->lblAverage->setText(QString::fromStdString(data.average));
    ui->lblPercentage->setText(QString::fromStdString(data.percentage));
}

void ProgressWidget::displayGauges(
    const std::vector<contracts::DailyProgress::GaugeValues>& data)
{
    for (size_t row = 0, ind = 0; row < numRows.value; ++row) {
        for (size_t col = 0; col < numColumns.value; ++col, ++ind) {
            QLayoutItem* item = ui->gaugeLayout->itemAtPosition(
                static_cast<int>(row), static_cast<int>(col));
            if (item) {
                const auto& [estimated, actual, emptyColor, filledColor] =
                    data[ind];
                dynamic_cast<Gauge*>(item->widget())
                    ->setData(actual, estimated, emptyColor, filledColor);
            }
        }
    }
}

void ProgressWidget::displayProgressBar(
    const contracts::DailyProgress::ProgressBarData& data)
{
    QPalette palette = ui->progressBarGoalProgress->palette();
    palette.setColor(QPalette::Highlight,
                     QColor{QString::fromStdString(data.filledColor)});
    palette.setColor(QPalette::Base,
                     QColor{QString::fromStdString(data.emptyColor)});
    ui->progressBarGoalProgress->setPalette(palette);
    ui->progressBarGoalProgress->setMaximum(data.estimated);
    ui->progressBarGoalProgress->setFormat(QString{"%1"}.arg(data.actual) +
                                           QString{"/%m"});
    if (data.estimated && data.actual != data.estimated)
        ui->progressBarGoalProgress->setValue(data.actual % data.estimated);
    else
        ui->progressBarGoalProgress->setValue(data.actual);
    ui->progressBarGoalProgress->setVisible(data.isVisible);
    ui->progressBarGoalProgress->repaint();
}

} // namespace sprint_timer::ui::qt_gui
