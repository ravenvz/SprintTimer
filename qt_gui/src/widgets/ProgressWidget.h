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

#ifndef POMODORO_PROGRESSWIDGET_H
#define POMODORO_PROGRESSWIDGET_H

#include "core/Distribution.h"
#include <QtWidgets/QFrame>
#include <ui_progress_widget.h>

namespace Ui {
class ProgressView;
}

class ProgressView : public QFrame {
    Q_OBJECT

public:
    ProgressView(int goal,
                 size_t numRows,
                 size_t numColumns,
                 double gaugeRelSize,
                 QWidget* parent);

    ~ProgressView();

    void setLegendTitle(const QString& title);

    void setLegendTotalCaption(const QString& caption);

    void setLegendAverageCaption(const QString& caption);

    void setLegendPercentageCaption(const QString& caption);

    void setLegendGoalCaption(const QString& caption);

    void setData(const Distribution<int>& distribution);

private:
    Ui::ProgressView* ui;
    int goal;
    size_t numRows;
    size_t numColumns;
    const double gaugeRelSize;

    void setupGauges();

    void fillGauges(const Distribution<int>& distribution);

    void updateProgressBar(int lastValue);

signals:
    void goalChanged(int goal);
};

QString formatDecimal(double decimal);

double percentage(int chunk, int total);


#endif // POMODORO_PROGRESSWIDGET_H
