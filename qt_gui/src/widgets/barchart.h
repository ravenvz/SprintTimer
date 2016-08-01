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
#ifndef BARCHART_H
#define BARCHART_H

#include <QEvent>
#include <QPen>
#include <QWidget>
#include <vector>

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
    // BarData(std::vector<double>& values, std::vector<QString>& labels);
    BarData(const std::vector<double>& values,
            const std::vector<QString>& labels);

    const BarDataItem& operator[](size_t idx) const;
    size_t size() const;

private:
    std::vector<BarDataItem> data;

    void normalize();
};

class BarChart : public QWidget {
    Q_OBJECT

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
    std::vector<QRectF> barRectangles;

    void computeAdaptiveSizes();
    void drawBars(QPainter& painter);
};

#endif // BARCHART_H