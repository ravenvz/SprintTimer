#include "barchart.h"
#include <QPainter>
#include <QMouseEvent>

#include <QDebug>


BarChart::BarChart(QWidget* parent) :
    QWidget(parent)
{

}

void BarChart::setData(const BarData& data) {
    barData = data;
    repaint();
}

void BarChart::setPen(QPen& barPen) {
    pen = barPen;
}

void BarChart::setBrush(QBrush& barBrush) {
    brush = barBrush;
}

void BarChart::paintEvent(QPaintEvent* event) {
    computeAdaptiveSizes();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    drawBars(painter);
}

void BarChart::drawBars(QPainter& painter) {
    double gapToWidthRatio = 0.4;
    double barWidth = availableRect.width() / (barData.size() * (1 + gapToWidthRatio) + gapToWidthRatio);
    double gapWidth = gapToWidthRatio * barWidth;

    double barHeightToMaxHeightRatio = 0.8;
    double legendHeightRatio = (1 - barHeightToMaxHeightRatio) / 2;
    double maxHeight = barHeightToMaxHeightRatio * availableRect.height();

    QPointF current = availableRect.bottomLeft();
    current.setY(current.y() - legendHeightRatio * availableRect.height());
    current.setX(current.x() + barWidth / 2 + gapWidth);
    QPen labelPen;
    painter.setBrush(brush);
    for (int i = 0; i < barData.size(); ++i) {
        painter.setPen(labelPen);
        QPointF legendOffsetPoint = QPointF(barWidth / 2, -legendHeightRatio * availableRect.height());
        painter.drawText(current - legendOffsetPoint, barData[i].label);
        QRectF barRect = QRectF {current.x() - barWidth / 2,
                                 current.y() - barData[i].normalizedValue * maxHeight,
                                 barWidth,
                                 barData[i].normalizedValue * maxHeight};
        painter.drawText(barRect.topLeft() - QPointF(0, legendHeightRatio / 2 * maxHeight),
                QString("%1").arg(barData[i].value, 1, 'f', 1, '0'));
        painter.setPen(pen);
        painter.drawRect(barRect);
        current.setX(current.x() + barWidth + gapWidth);
    }
}

void BarChart::computeAdaptiveSizes() {
    const QRectF totalSizeRect = QRectF(QPointF(0, 0), this->size());
    const QPointF center = totalSizeRect.center();
    const double margin = 0.1;
    const double availableWidth = (1 - margin) * totalSizeRect.width();
    const double availableHeight = (1 - margin) * totalSizeRect.height();
    availableRect = QRectF {center.x() - availableWidth / 2,
                            center.y() - availableHeight / 2,
                            availableWidth,
                            availableHeight};
}

BarData::BarData() {

}

BarData::BarData(QVector<double>& values, QVector<QString>& labels) {
    int length = std::min(values.size(), labels.size());
    for (int i = 0; i < length; ++i) {
        data.push_back(BarDataItem {labels[i], values[i], 0.0});
    }
    normalize();
}

void BarData::normalize() {
    double maxValue = std::max_element(data.begin(), data.end(), [](auto entry1, auto entry2) {
            return entry1.value < entry2.value;
    })->value;
    std::transform(data.begin(), data.end(), data.begin(), [maxValue](auto entry) {
        return BarDataItem {entry.label, entry.value, entry.value / maxValue};
    }); 
}

const BarDataItem& BarData::operator[](std::size_t idx) const {
    return data[idx];
}

const int BarData::size() const {
    return data.size();
}



