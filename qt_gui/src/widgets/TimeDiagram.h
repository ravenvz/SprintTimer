/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

#include "date_wrapper/TimeSpan.h"
#include <QEvent>
#include <QWidget>
#include <vector>

using dw::TimeSpan;

/* Displays vector of TimeSpan as a diagram.
 *
 * Diagram looks like a round clock that can display 24-hours instead
 * of 12 with resolution of 1 hour.
 * Top point corresponds to midnight, right point to 6:00,
 * bottom point to 12:00, left point to 18:00.
 *
 * Time spans are painted as a sector on this diagram. If multiple time spans
 * are intersecting, intersection is painted in brighter color.
 */
class TimeDiagram : public QWidget {
    Q_OBJECT

public:
    explicit TimeDiagram(QWidget* parent);

    /* Set vector of time intervals to display as a diagram. */
    void setIntervals(std::vector<TimeSpan> newIntervals);

protected:
    void paintEvent(QPaintEvent*) override;

private:
    const QColor timeSpanColor = QColor::fromRgb(246, 61, 13, 20);
    std::vector<TimeSpan> timeSpans;
    QRectF totalSizeRect;
    QRectF diagramRect;
    double diagramRadius{0};
    double tickOffset{0};
    double longTickLength{0};
    double shortTickLength{0};

    void computeAdaptiveSizes();

    void drawDiagramCanvas(QPainter& painter);

    void drawIntervals(QPainter& painter);
};

#endif /* end of include guard: TIMEDIAGRAM_H */
