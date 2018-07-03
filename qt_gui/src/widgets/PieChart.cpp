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
#include "qt_gui/widgets/PieChart.h"

namespace sprint_timer::ui::qt_gui {

PieChart::PieChart(QWidget* parent)
    : IStatisticalChart(parent)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
}

void PieChart::setData(
    const std::vector<IStatisticalChart::LabelValuePair>& dataToDisplay)
{
    data = dataToDisplay;
    activeSliceInd = std::optional<size_t>();
    repaint();
}

void PieChart::paintEvent(QPaintEvent*)
{
    computeAdaptiveSizes();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(borderColor);
    constexpr int degreesInCircle = 360;
    // QPainter API requires degrees to be specified in 1/16 of degree.
    constexpr int qtDegPrecision = 16;
    constexpr int qtFullCircle = degreesInCircle * qtDegPrecision;

    double offset = 0; // Offset to next slice in degrees.

    for (size_t sliceInd = 0; sliceInd < data.size(); ++sliceInd) {
        const bool isCurrentlyActiveSlice
            = activeSliceInd && (*activeSliceInd == sliceInd);
        const double currentValue = data[sliceInd].second;

        painter.setBrush(brushes[sliceInd % brushes.size()]);

        if (isCurrentlyActiveSlice) {
            QPointF offsetPoint
                = computeOffsetPoint(currentValue * degreesInCircle, offset);
            painter.translate(offsetPoint);
            painter.drawPie(pieRect,
                            int(offset * qtDegPrecision),
                            int(currentValue * qtFullCircle));
            painter.resetTransform();
        }
        else {
            painter.drawPie(pieRect,
                            int(offset * qtDegPrecision),
                            int(currentValue * qtFullCircle));
        }

        offset += currentValue * degreesInCircle;
    }
}

QPointF PieChart::computeOffsetPoint(double current, double offset)
{
    double angle = offset + current / 2;
    double angleRads = angle * pi / 180;
    return QPointF{(expandedShiftLength * cos(angleRads)),
                   (-expandedShiftLength * sin(angleRads))};
}

void PieChart::computeAdaptiveSizes()
{
    widgetRect = QRectF(QPointF(0, 0), this->size());
    QPointF center = widgetRect.center();
    double expandedSliceRelativeDiameter
        = 0.98 * std::min(widgetRect.width(), widgetRect.height());
    double diagramRelativeDiameter
        = expandedSliceRelativeDiameter - 0.1 * expandedSliceRelativeDiameter;
    expandedShiftLength
        = (expandedSliceRelativeDiameter - diagramRelativeDiameter) / 2;
    pieRect = QRectF{center.x() - diagramRelativeDiameter / 2,
                     center.y() - diagramRelativeDiameter / 2,
                     diagramRelativeDiameter,
                     diagramRelativeDiameter};
}

void PieChart::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        onLeftMouseClick(event->pos());
    }
}

void PieChart::onLeftMouseClick(const QPoint& pos)
{
    double angle = QLineF(pieRect.center(), pos).angle();
    double offset = 0;
    for (size_t sliceInd = 0; sliceInd < data.size(); ++sliceInd) {
        offset += data[sliceInd].second * 360;
        if (angle < offset) {
            emit partClicked(sliceInd);
            break;
        }
    }
}

void PieChart::togglePartActive(size_t sliceIndex)
{
    if (activeSliceInd && (*activeSliceInd == sliceIndex))
        activeSliceInd = std::optional<size_t>();
    else
        activeSliceInd = sliceIndex;
    repaint();
}

} // namespace sprint_timer::ui::qt_gui

