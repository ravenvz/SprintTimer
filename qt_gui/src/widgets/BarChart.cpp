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
#include "qt_gui/widgets/BarChart.h"
#include <QMouseEvent>
#include <QPainter>

namespace sprint_timer::ui::qt_gui {

BarChart::BarChart(QWidget* parent)
    : QWidget(parent)
{
}

void BarChart::setData(const BarData& data)
{
    barData = data;
    repaint();
}

void BarChart::setPen(QPen& barPen) { pen = barPen; }

void BarChart::setBrush(QBrush& barBrush) { brush = barBrush; }

void BarChart::paintEvent(QPaintEvent*)
{
    const QRectF totalSizeRect = rect();
    const QPointF rectCenter = totalSizeRect.center();
    const double margin = 0.1;
    const double availableWidth = (1 - margin) * totalSizeRect.width();
    const double availableHeight = (1 - margin) * totalSizeRect.height();
    availableRect = QRectF{rectCenter.x() - availableWidth / 2,
                           rectCenter.y() - availableHeight / 2,
                           availableWidth,
                           availableHeight};
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawBars(painter);
}

void BarChart::drawBars(QPainter& painter)
{
    constexpr double gapToWidthRatio = 0.4;
    constexpr double barHeightToMaxHeightRatio = 0.8;

    const double barWidth = availableRect.width()
        / (barData.size() * (1 + gapToWidthRatio) + gapToWidthRatio);
    const double gapWidth = gapToWidthRatio * barWidth;
    const double legendHeightRatio = (1 - barHeightToMaxHeightRatio) / 2;
    const double maxHeight = barHeightToMaxHeightRatio * availableRect.height();

    QPointF current = availableRect.bottomLeft();
    current.setY(current.y() - legendHeightRatio * availableRect.height());
    current.setX(current.x() + barWidth / 2 + gapWidth);

    const QPen labelPen;
    painter.setBrush(brush);

    for (size_t i = 0; i < barData.size(); ++i) {
        painter.setPen(labelPen);
        QPointF legendOffsetPoint = QPointF(
            barWidth / 2, -legendHeightRatio * availableRect.height());
        painter.drawText(current - legendOffsetPoint, barData[i].label);
        QRectF barRect
            = QRectF{current.x() - barWidth / 2,
                     current.y() - barData[i].normalizedValue * maxHeight,
                     barWidth,
                     barData[i].normalizedValue * maxHeight};
        painter.drawText(barRect.topLeft()
                             - QPointF(0, legendHeightRatio / 2 * maxHeight),
                         QString("%1").arg(barData[i].value, 1, 'f', 1, '0'));
        painter.setPen(pen);
        painter.drawRect(barRect);
         current.setX(current.x() + barWidth + gapWidth);
    }
}

BarData::BarData() {}

BarData::BarData(const std::vector<double>& values,
                 const std::vector<QString>& labels)
{
    size_t length = std::min(values.size(), labels.size());
    data.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        data.push_back(BarDataItem{labels[i], values[i], 0.0});
    }
    normalize();
}

void BarData::normalize()
{
    double maxValue = std::max_element(data.begin(),
                                       data.end(),
                                       [](auto entry1, auto entry2) {
                                           return entry1.value < entry2.value;
                                       })
                          ->value;
    std::transform(
        data.begin(), data.end(), data.begin(), [maxValue](auto entry) {
            return BarDataItem{
                entry.label, entry.value, (maxValue > 0.0) ? entry.value / maxValue : 0.0};
        });
}

const BarDataItem& BarData::operator[](size_t idx) const { return data[idx]; }

size_t BarData::size() const { return data.size(); }

} // namespace sprint_timer::ui::qt_gui

