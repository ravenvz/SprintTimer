#include "widgets/PieChart.h"

PieChart::PieChart(QWidget* parent)
    : IStatisticalChart(parent)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
}

void PieChart::setData(
    const std::vector<IStatisticalChart::LabelValuePair>& dataToDisplay)
{
    data = dataToDisplay;
    activeSliceInd = optional<size_t>();
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
    // TODO should really test this one.
    double angle = offset + current / 2;
    double angleRads = angle * pi / 180;
    double x = expandedShiftLength * cos(angleRads);
    double y = expandedShiftLength * sin(angleRads);
    if (0 <= angle && angle <= 90)
        y = -y;
    else if (90 < angle && angle <= 180)
        y = -y;
    else if (180 < angle && angle <= 270)
        y = -y;
    else
        y = -y;
    return QPointF{x, y};
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
    expandedRect = QRectF{center.x() - expandedSliceRelativeDiameter / 2,
                          center.y() - expandedSliceRelativeDiameter / 2,
                          expandedSliceRelativeDiameter,
                          expandedSliceRelativeDiameter};
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

// void PieChart::onSliceSelectionChanged(size_t sliceIndex)
// {
//     selectedSlice
//         ? selected selectedSliceIndex.emplace(
//               selectedSliceIndex == sliceIndex ? -1 : sliceIndex);
//     repaint();
// }

void PieChart::togglePartActive(size_t sliceIndex)
{
    if (activeSliceInd && (*activeSliceInd == sliceIndex))
        activeSliceInd = optional<size_t>();
    else
        activeSliceInd = sliceIndex;
    repaint();
    // emit sliceSelectionChanged(sliceIndex);
}