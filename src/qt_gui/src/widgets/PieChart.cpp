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
#include "qt_gui/widgets/PieChart.h"
#include <array>
#include <cmath>
#include <numbers>

namespace {

constexpr int degreesInCircle{360};

// QPainter API requires degrees to be specified in 1/16 of degree.
constexpr int qtDegPrecision{16};

constexpr int qtFullCircle{degreesInCircle * qtDegPrecision};

// Scale of expanded diagram to shortest side of widget's bounding rectangle.
constexpr double expandedScale{0.99};

// Scale of shrinked diagram to shortest side of widget's bounding rectangle.
constexpr double shrinkedScale{0.85};

/* Color for chart and slices borders. */
const QPen borderColor{Qt::gray};

struct PainterTransformScope {

    explicit PainterTransformScope(QPainter& painter_);

    ~PainterTransformScope();

private:
    QPainter& painter;
};

std::pair<QRectF, double> adaptedDimensions(const QRectF& boundingRect);

QPointF computeOffsetPoint(double angle, double expansionLength);

void drawSlice(QPainter& painter,
               const QRectF& pieRect,
               double startAngle,
               double stopAngle);

} // namespace

namespace sprint_timer::ui::qt_gui {

PieChart::PieChart(QWidget* parent_)
    : QWidget{parent_}
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
}

void PieChart::setData(std::span<const DataEntry> dataToDisplay)
{
    angles.clear();
    angles.resize(dataToDisplay.size());
    brushes.clear();
    brushes.resize(dataToDisplay.size());

    std::ranges::transform(
        dataToDisplay,
        begin(brushes),
        [](const auto& color) { return QBrush(QColor(color.c_str())); },
        &DataEntry::colorCode);

    std::transform_inclusive_scan(cbegin(dataToDisplay),
                                  cend(dataToDisplay),
                                  begin(angles),
                                  std::plus<double>{},
                                  [](const DataEntry& value) {
                                      return value.percentage * degreesInCircle;
                                  });
    activeSliceInd = std::nullopt;
    repaint();
}

void PieChart::togglePartActive(size_t sliceIndex)
{
    activeSliceInd = isSelected(sliceIndex) ? std::nullopt
                                            : std::optional<size_t>(sliceIndex);
    repaint();
}

void PieChart::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(borderColor);

    const auto [pieRect, expansionLength] = adaptedDimensions(this->rect());

    auto draw = [&, startAngle = 0.0, sliceInd = 0UL](double angle) mutable {
        painter.setBrush(brushes[sliceInd]);
        PainterTransformScope transformScope{painter};
        if (isSelected(sliceInd)) {
            painter.translate(
                computeOffsetPoint((startAngle + angle) / 2, expansionLength));
        }
        drawSlice(painter, pieRect, startAngle, angle);
        ++sliceInd;
        startAngle = angle;
    };

    std::ranges::for_each(angles, draw);
}

void PieChart::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        onLeftMouseClick(event->pos());
    }
}

void PieChart::onLeftMouseClick(const QPoint& pos)
{
    const double angle{QLineF(rect().center(), pos).angle()};
    if (const auto it = std::ranges::lower_bound(angles, angle);
        it != cend(angles)) {
        emit partClicked(static_cast<size_t>(std::distance(begin(angles), it)));
    }
}

bool PieChart::isSelected(size_t index) const
{
    return activeSliceInd and (activeSliceInd.value() == index);
}

} // namespace sprint_timer::ui::qt_gui

namespace {

PainterTransformScope::PainterTransformScope(QPainter& painter_)
    : painter{painter_}
{
}

PainterTransformScope::~PainterTransformScope() { painter.resetTransform(); }

std::pair<QRectF, double> adaptedDimensions(const QRectF& boundingRect)
{
    const QPointF center{boundingRect.center()};
    const double shortestSide{
        std::min(boundingRect.width(), boundingRect.height())};
    const double expandedPieRadius{expandedScale * shortestSide / 2};
    const double shrinkedPieRadius{shrinkedScale * shortestSide / 2};
    return {QRectF{center.x() - shrinkedPieRadius,
                   center.y() - shrinkedPieRadius,
                   2 * shrinkedPieRadius,
                   2 * shrinkedPieRadius},
            expandedPieRadius - shrinkedPieRadius};
};

QPointF computeOffsetPoint(double angle, double expansionLength)
{
    const double angleRads{angle * std::numbers::pi / 180};
    return QPointF{expansionLength * cos(angleRads),
                   -expansionLength * sin(angleRads)};
};

void drawSlice(QPainter& painter,
               const QRectF& pieRect,
               double startAngle,
               double stopAngle)
{
    painter.drawPie(
        pieRect,
        static_cast<int>(startAngle * qtDegPrecision),
        static_cast<int>((stopAngle - startAngle) * qtDegPrecision));
};

} // namespace

