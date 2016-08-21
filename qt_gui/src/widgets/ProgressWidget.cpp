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

#include "ProgressWidget.h"
#include "ui_progress_widget.h"
#include "widgets/gauge.h"
#include <QtWidgets/QGridLayout>

namespace ProgressBarColors {
const QColor targetGoalReached = QColor("#6baa15");
const QColor overwork = Qt::red;
const QColor workInProgress = Qt::gray;
};

ProgressView::ProgressView(int goal,
                           size_t numRows,
                           size_t numColumns,
                           double gaugeRelSize,
                           QWidget* parent)
    : QFrame{parent}
    , ui{new Ui::ProgressView}
    , goal{goal}
    , numRows{numRows}
    , numColumns{numColumns}
    , gaugeRelSize{gaugeRelSize}
{
    ui->setupUi(this);

    ui->spinBoxGoal->setValue(goal);

    connect(ui->spinBoxGoal,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            [&](int goal) {
                this->goal = goal;
                emit goalChanged(goal);
            });

    setupGauges();
    updateProgressBar(0);
}

ProgressView::~ProgressView() { delete ui; }

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

void ProgressView::setLegendGoalCaption(const QString& caption)
{
    ui->lblGoalCaption->setText(caption);
}

void ProgressView::setData(const Distribution<int>& distribution)
{
    ui->lblProgress->setText(QString("%1").arg(distribution.getTotal()));
    auto expected = goal * distribution.getNumBins();
    auto actual = distribution.getTotal();
    if (actual > expected) {
        ui->lblLeftCaption->setText("Overwork:");
        ui->lblLeft->setText(QString{"%1"}.arg(actual - expected));
    } else {
        ui->lblLeftCaption->setText("Left to complete:");
        ui->lblLeft->setText(QString("%1").arg(expected - actual));
    }
    ui->lblAverage->setText(formatDecimal(distribution.getAverage()));
    ui->lblPercentage->setText(QString("%1%").arg(formatDecimal(
        percentage(distribution.getTotal(),
                   static_cast<int>(goal * distribution.getNumBins())))));
    fillGauges(distribution);
    updateProgressBar(distribution.getBinValue(distribution.getNumBins() - 1));
}

void ProgressView::setupGauges()
{
    for (auto row = 0; row < numRows; ++row) {
        for (auto col = 0; col < numColumns; ++col) {
            ui->gaugeLayout->addWidget(
                new Gauge(0, goal, gaugeRelSize, this), row, col);
        }
    }
}

void ProgressView::fillGauges(const Distribution<int>& distribution)
{
    QLayoutItem* item;
    for (size_t row = 0, ind = 0; row < numRows; ++row) {
        for (size_t col = 0; col < numColumns; ++col, ++ind) {
            item = ui->gaugeLayout->itemAtPosition(static_cast<int>(row),
                                                   static_cast<int>(col));
            if (item) {
                dynamic_cast<Gauge*>(item->widget())
                    ->setData(distribution.getBinValue(ind), goal);
            }
        }
    }
}

void ProgressView::updateProgressBar(int lastValue)
{
    auto bar = ui->progressBarGoalProgress;
    bar->setMaximum(goal);
    bar->setFormat("%v/%m");
    bar->hide();
    if (goal == 0) {
        return;
    }
    QPalette p = bar->palette();
    if (lastValue == goal) {
        p.setColor(QPalette::Highlight, ProgressBarColors::targetGoalReached);
        p.setColor(QPalette::Base, ProgressBarColors::targetGoalReached);
    }
    else if (lastValue > goal) {
        p.setColor(QPalette::Highlight, ProgressBarColors::overwork);
        p.setColor(QPalette::Base, ProgressBarColors::targetGoalReached);
        bar->setFormat(QString("%1").arg(lastValue) + QString("/%m"));
        lastValue %= goal;
    }
    else {
        p.setColor(QPalette::Highlight, ProgressBarColors::workInProgress);
        p.setColor(QPalette::Base, Qt::white);
    }
    bar->setPalette(p);
    bar->setValue(lastValue);
    bar->show();
}

QString formatDecimal(double decimal)
{
    return QString("%1").arg(decimal, 2, 'f', 2, '0');
}

double percentage(int chunk, int total)
{
    return total != 0 ? static_cast<double>(chunk) * 100 / total : 0;
}
