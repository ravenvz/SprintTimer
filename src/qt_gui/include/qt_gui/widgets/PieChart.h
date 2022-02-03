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
#ifndef PIECHART_H_65LSH0MP
#define PIECHART_H_65LSH0MP

#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <optional>
#include <span>

namespace sprint_timer::ui::qt_gui {

/* Widget that displays data as an exploding pie chart.
 *
 * It provides means to select individual slice of the pie chart
 * by either invoking method call or by user clicking the desired
 * slice. Slice then 'expands' - that is - moved from a center of
 * the chart (like a pie slice).
 * When user selects slice with left mouse click, partClicked(size_t)
 * signal is emitted, indicating an index of the selected slice. */
class PieChart : public QWidget {

    Q_OBJECT

public:
    struct DataEntry {
        double percentage;
        std::string colorCode;
    };

    explicit PieChart(QWidget* parent);

    void setData(std::span<const DataEntry> data);

    void togglePartActive(size_t sliceIndex);

private:
    std::optional<size_t> activeSliceInd;
    std::vector<double> angles;
    std::vector<QBrush> brushes;

    void paintEvent(QPaintEvent*) override;

    void mousePressEvent(QMouseEvent* event) override;

    /* Handle left mouse click event. Emit partClicked(size_t index) with
     * index of clicked slice. */
    void onLeftMouseClick(const QPoint& pos);

    [[nodiscard]] bool isSelected(size_t index) const;

signals:
    void partClicked(size_t);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: PIECHART_H_65LSH0MP */
