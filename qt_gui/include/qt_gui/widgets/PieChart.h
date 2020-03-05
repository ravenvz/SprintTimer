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

#include "qt_gui/widgets/IStatisticalChart.h"
#include <QMouseEvent>
#include <QPainter>
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
class PieChart : public QWidget {

    Q_OBJECT

public:
    struct LabelData {
        std::string name;
        double percentage;
        std::string colorCode;
    };

    explicit PieChart(QWidget* parent);

    /* Set data that should be displayed as a pie chart.
     * Labels are ignored by this implementation and sum of all
     * values is treated as 100% */
    void setData(const std::vector<LabelData>& data);

    /* Toggle slice at index as expanded/normal. */
    void togglePartActive(size_t sliceIndex);

protected:
    /* Overriden to implement drawing of the chart. */
    void paintEvent(QPaintEvent*);

    /* Overriden to capture left mouse click on the chart. */
    void mousePressEvent(QMouseEvent* event);

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
    std::vector<LabelData> data;

    /* Compute sizes of bounding rectangles. */
    void computeAdaptiveSizes();

    /* Return point where expanded slice center point should be. */
    QPointF computeOffsetPoint(double current, double offset);

    /* Handle left mouse click event. Emit partClicked(size_t index) with
     * index of clicked slice. */
    void onLeftMouseClick(const QPoint& pos);

signals:
    void partClicked(size_t);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: PIECHART_H_65LSH0MP */
