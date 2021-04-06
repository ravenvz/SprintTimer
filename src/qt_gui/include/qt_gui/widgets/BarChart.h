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
#ifndef BARCHART_H
#define BARCHART_H

#include <QEvent>
#include <QPen>
#include <QWidget>
#include <vector>

namespace sprint_timer::ui::qt_gui {

struct BarDataItem {
    QString label;
    double value;
    double normalizedValue;
};

class BarData {

public:
    BarData();

    /* Both vectors should be of equal size, otherwise data would
     * be set only for number of bars equal to shortest vector */
    BarData(const std::vector<double>& values,
            const std::vector<QString>& labels);

    const BarDataItem& operator[](size_t idx) const;
    size_t size() const;

private:
    std::vector<BarDataItem> data;

    void normalize();
};

class BarChart : public QWidget {

public:
    explicit BarChart(QWidget* parent);
    void setData(const BarData& data);
    void setPen(QPen& barPen);
    void setBrush(QBrush& barBrush);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    BarData barData;
    QPen pen;
    QBrush brush;
    QRectF availableRect;

    void drawBars(QPainter& painter);
};

} // namespace sprint_timer::ui::qt_gui

#endif // BARCHART_H
