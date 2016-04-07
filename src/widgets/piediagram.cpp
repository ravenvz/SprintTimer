#include <QtGui/qpainter.h>
#include <QHBoxLayout>
#include "piediagram.h"
#include <iostream>


PieDiagram::PieDiagram(QWidget* parent)
    : QWidget(parent)
{
    // QWidget takes ownership of objects allocated below,
    // so no need to delete them manually
    QHBoxLayout* layout = new QHBoxLayout();
    legendLayout = new QVBoxLayout();
    diagram = new PieChart(this);
    labelLegendTitle = new QLabel();
    legendLayout->addWidget(labelLegendTitle); // Takes ownership
    layout->addLayout(legendLayout); // Takes ownership
    layout->addWidget(diagram); // Takes ownership
    setLayout(layout); // QWidget takes ownership of layout
    connect(diagram,
            SIGNAL(partClicked(size_t)),
            this,
            SLOT(onPieSliceClicked(size_t)));
}

PieDiagram::~PieDiagram() {}

void PieDiagram::setData(const std::vector<TagCount>& data)
{
    selectedSliceIndex = optional<size_t>();
    addLegendLabels(data);
    legendLayout->addStretch(1);
    diagram->setData(data);
}

void PieDiagram::setLegendTitle(const QString& title)
{
    labelLegendTitle->setText(title);
    labelLegendTitle->setVisible(true);
}

void PieDiagram::setFont(QFont font) { labelLegendTitle->setFont(font); }

void PieDiagram::onPieSliceClicked(size_t sliceIndex)
{
    const bool selectedNewItem
        = selectedSliceIndex && (*selectedSliceIndex != sliceIndex);
    const bool selectedSameItem = selectedSliceIndex
        && (*selectedSliceIndex == sliceIndex)
        && labels[sliceIndex]->isSelected();

    if (selectedNewItem) {
        labels[*selectedSliceIndex]->toggleSelected();
        selectedSliceIndex = sliceIndex;
    }

    selectedSliceIndex = (selectedSameItem) ? optional<size_t>() : sliceIndex;

    labels[sliceIndex]->toggleSelected();
    diagram->togglePartActive(sliceIndex);

    emit sliceSelectionChanged(sliceIndex);
}

void PieDiagram::onLegendItemClicked(size_t itemIndex)
{
    onPieSliceClicked(itemIndex);
}

// TODO this method is just asks to be refactored out to separate class
void PieDiagram::addLegendLabels(const std::vector<TagCount>& data)
{
    const auto& labelsRef = labels;
    for (LegendLabel* label : labelsRef) {
        legendLayout->removeWidget(label);
        delete label;
    }
    labels.clear();
    for (size_t labelInd = 0; labelInd < data.size(); ++labelInd) {
        LegendLabel* label = new LegendLabel(
            QString::fromStdString(data[labelInd].first), labelInd);
        legendLayout->addWidget(label); // Takes ownership
        label->setVisible(true);
        connect(label,
                SIGNAL(clicked(size_t)),
                this,
                SLOT(onLegendItemClicked(size_t)));
        labels.push_back(label);
    }
}

LegendLabel::LegendLabel(const QString& text, size_t itemIndex, QWidget* parent)
    : QLabel(parent)
    , itemIndex(itemIndex)
{
    this->setText(text);
}

LegendLabel::~LegendLabel() {}

void LegendLabel::toggleSelected()
{
    selected = !selected;
    if (selected)
        setStyleSheet("QLabel { color: green; }");
    else
        setStyleSheet("QLabel { color: black; }");
}

bool LegendLabel::isSelected() const { return selected; }

void LegendLabel::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        emit clicked(itemIndex);
}
