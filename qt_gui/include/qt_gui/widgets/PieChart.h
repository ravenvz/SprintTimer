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
#ifndef PIECHART_H_65LSH0MP
#define PIECHART_H_65LSH0MP

#include <QPainter>
#include <QMouseEvent>
#include "qt_gui/widgets/IStatisticalChart.h"
#include <cmath>
#include <optional>

namespace sprint_timer::ui::qt_gui {

/* Widget that displays data as an exploding pie chart.
 *
 * It provides means to select individual slice of the pie chart
 * by either invoking method call or by user clicking the desired
 * slice. Slice then 'expands' - that is - moved from a center of
 * the chart.
 * When user selects slice with left mouse click, partClicked(size_t)
 * signal is emitted, indicating an index of the selected slice. */
class PieChart : public IStatisticalChart {
    Q_OBJECT

public:
    explicit PieChart(QWidget* parent);

    /* Set data that should be displayed as a pie chart.
     * Labels are ignored by this implementation and sum of all
     * values is treated as 100% */
    void setData(const std::vector<IStatisticalChart::LabelValuePair>&
                     dataToDisplay) override;

    /* Toggle slice at index as expanded/normal. */
    void togglePartActive(size_t sliceIndex) override;

protected:
    /* Overriden to implement drawing of the chart. */
    void paintEvent(QPaintEvent*) override;

    /* Overriden to capture left mouse click on the chart. */
    void mousePressEvent(QMouseEvent* event) override;

private:
    /* Rect that bounds this widget for this widget. */
    QRectF widgetRect;
    /* Rect that bounds chart in non-expanded state. */
    QRectF pieRect;
    /* Distance by which expanded slice is shifted from center of the chart. */
    double expandedShiftLength;
    /* Index of currently active slice or nothing if no slice selected.
     */
    std::optional<size_t> activeSliceInd;
    /* Data that is to be displayed as a pie chart. */
    std::vector<LabelValuePair> data;
    /* Constant pi. */
    const double pi{std::acos(-1)};
    /* Colors for the slices. */
    std::vector<QBrush> brushes{QBrush(QColor("#28245a")),
                                QBrush(QColor("#73c245")),
                                QBrush(QColor("#ea6136")),
                                QBrush(QColor("#1d589b")),
                                QBrush(QColor("#d62a36")),
                                QBrush(QColor("#401b60")),
                                QBrush(QColor("#f8cd32")),
                                QBrush(QColor("#258bc8")),
                                QBrush(QColor("#087847"))};
    /* Color for chart and slices borders. */
    const QPen borderColor{Qt::gray};

    /* Compute sizes of bounding rectangles. */
    void computeAdaptiveSizes();

    /* Return point where expanded slice center point should be. */
    QPointF computeOffsetPoint(double current, double offset);

    /* Handle left mouse click event. Emit partClicked(size_t index) with
     * index of clicked slice. */
    void onLeftMouseClick(const QPoint& pos);
};

} // namespace sprint_timer::ui::qt_gui


#endif /* end of include guard: PIECHART_H_65LSH0MP */
