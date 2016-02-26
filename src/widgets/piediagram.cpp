#include <QtGui/qpainter.h>
#include <QHBoxLayout>
#include "piediagram.h"

PieDiagram::PieDiagram(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    legendLayout = new QVBoxLayout();
    diagram = new Diagram(this);
    labelLegendTitle = new QLabel();
    legendLayout->addWidget(labelLegendTitle);
    layout->addLayout(legendLayout);
    layout->addWidget(diagram);
    setLayout(layout);
    connect(diagram, SIGNAL(sliceSelectionChanged(int)), this,
        SLOT(onSliceSelectionChanged(int)));
}

PieDiagram::~PieDiagram() { delete diagram; }

void PieDiagram::setData(QVector<Slice>& data)
{
    selectedSlice = -1;
    for (LegendLabel* label : labels) {
        legendLayout->removeWidget(label);
        delete label;
    }
    labels.clear();
    for (int i = 0; i < data.size(); ++i) {
        LegendLabel* label = new LegendLabel(data[i].first, i);
        legendLayout->addWidget(label);
        label->setVisible(true);
        connect(
            label, SIGNAL(clicked(int)), this, SLOT(onLegendItemClicked(int)));
        labels << label;
    }
    legendLayout->addStretch(1);
    diagram->setData(data);
}

void PieDiagram::setLegendTitle(QString title)
{
    labelLegendTitle->setText(title);
    labelLegendTitle->setVisible(true);
}

void PieDiagram::setFont(QFont font) { labelLegendTitle->setFont(font); }

void PieDiagram::onSliceSelectionChanged(int sliceIndex)
{
    if (selectedSlice != -1)
        labels[selectedSlice]->setStyleSheet("QLabel { color: black; }");
    if (sliceIndex != -1 && sliceIndex != selectedSlice)
        labels[sliceIndex]->setStyleSheet("QLabel { color: green; }");
    selectedSlice = selectedSlice == sliceIndex ? -1 : sliceIndex;
    emit sliceSelectionChanged(sliceIndex);
}

void PieDiagram::onLegendItemClicked(int itemIndex)
{
    diagram->setSelectedSlice(itemIndex);
}

Diagram::Diagram(QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    connectSlots();
}

void Diagram::connectSlots()
{
    connect(this, SIGNAL(sliceSelectionChanged(int)), this,
        SLOT(onSliceSelectionChanged(int)));
}

void Diagram::setData(QVector<Slice>& data)
{
    selectedPieIndex = -1;
    sortedData.clear();
    std::transform(data.begin(), data.end(), std::back_inserter(sortedData),
        [](const auto& entry) {
            return std::make_pair(entry.first, entry.second * 360);
        });
    repaint();
}

void Diagram::paintEvent(QPaintEvent*)
{
    computeAdaptiveSizes();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setWidthF(1.2);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    //    painter.drawEllipse(diagramRect);
    //    painter.drawEllipse(expandedSliceRect);

    painter.setPen(QPen(Qt::gray));

    double offset = 0;
    for (int i = 0; i < sortedData.size(); ++i) {
        painter.setBrush(brushes[i % brushes.size()]);
        if (i == selectedPieIndex) {
            QPointF offsetPoint
                = computeOffsetPoint(sortedData[i].second, offset);
            painter.translate(offsetPoint);
            painter.drawPie(
                diagramRect, int(offset * 16), int(sortedData[i].second * 16));
            painter.resetTransform();
        }
        else {
            painter.drawPie(
                diagramRect, int(offset * 16), int(sortedData[i].second * 16));
        }
        offset += sortedData[i].second;
    }
}

QPointF Diagram::computeOffsetPoint(double current, double offset)
{
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

void Diagram::computeAdaptiveSizes()
{
    totalSizeRect = QRectF(QPointF(0, 0), this->size());
    QPointF center = totalSizeRect.center();
    double expandedSliceRelativeDiameter
        = 0.98 * std::min(totalSizeRect.width(), totalSizeRect.height());
    double diagramRelativeDiameter
        = expandedSliceRelativeDiameter - 0.1 * expandedSliceRelativeDiameter;
    expandedShiftLength
        = (expandedSliceRelativeDiameter - diagramRelativeDiameter) / 2;
    expandedSliceRect = QRectF{center.x() - expandedSliceRelativeDiameter / 2,
        center.y() - expandedSliceRelativeDiameter / 2,
        expandedSliceRelativeDiameter, expandedSliceRelativeDiameter};
    diagramRect = QRectF{center.x() - diagramRelativeDiameter / 2,
        center.y() - diagramRelativeDiameter / 2, diagramRelativeDiameter,
        diagramRelativeDiameter};
}

void Diagram::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        updateSelectedSliceIndex(event->pos());
    }
}

void Diagram::updateSelectedSliceIndex(const QPoint& pos)
{
    double angle = QLineF(diagramRect.center(), pos).angle();
    double offset = 0;
    for (int i = 0; i < sortedData.size(); ++i) {
        offset += sortedData[i].second;
        if (angle < offset) {
            emit sliceSelectionChanged(i);
            break;
        }
    }
}

void Diagram::onSliceSelectionChanged(int sliceIndex)
{
    selectedPieIndex = selectedPieIndex == sliceIndex ? -1 : sliceIndex;
    repaint();
}

void Diagram::setSelectedSlice(int sliceIndex)
{
    emit sliceSelectionChanged(sliceIndex);
}

LegendLabel::LegendLabel(const QString& text, int itemIndex, QWidget* parent)
    : QLabel(parent)
    , itemIndex(itemIndex)
{
    this->setText(text);
}

LegendLabel::~LegendLabel() {}

void LegendLabel::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked(itemIndex);
}
