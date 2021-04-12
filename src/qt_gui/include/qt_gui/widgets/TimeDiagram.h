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
#ifndef TIMEDIAGRAM_H
#define TIMEDIAGRAM_H

#include <QEvent>
#include <QWidget>
#include "date_wrapper/date_wrapper.h"
#include <vector>

namespace sprint_timer::ui::qt_gui {

/* Displays vector of time ranges as a diagram.
 *
 * Diagram looks like a round clock that can has 24 hours indicators, as opposed
 * to classic 12-hours clock.
 * Top point corresponds to midnight,
 * right point to 6:00,
 * bottom point to 12:00,
 * left point to 18:00.
 *
 * Time ranges spans are painted as a sector on this diagram. If multiple ranges
 * are intersecting, intersection is painted in brighter color.
 */
class TimeDiagram : public QWidget {

public:
    explicit TimeDiagram(QWidget* parent);

    void setIntervals(std::vector<dw::DateTimeRange> newIntervals);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    struct DrawingParams {
        DrawingParams(const QRectF& totalSizeRect);

        DrawingParams() = default;

        QRectF diagramRect;
        double diagramRadius{0};
        double tickOffset{0};
        double longTickLength{0};
        double shortTickLength{0};
    };

    const QColor timeSpanColor = QColor::fromRgb(246, 61, 13, 20);
    std::vector<dw::DateTimeRange> timeSpans;
    DrawingParams drawingParams;

    void drawDiagramCanvas(QPainter& painter);

    void drawIntervals(QPainter& painter);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: TIMEDIAGRAM_H */
