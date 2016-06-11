#include <QtGui/qpainter.h>
#include <QHBoxLayout>
#include "widgets/DistributionDiagram.h"
#include <iostream>


DistributionDiagram::DistributionDiagram(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    diagram = new PieChart(this);
    legend = new SimpleLegend(this);
    layout->addWidget(legend);
    layout->addWidget(diagram);
    setLayout(layout); // QWidget takes ownership of layout
    connect(diagram,
            SIGNAL(partClicked(size_t)),
            this,
            SLOT(onChartPartClicked(size_t)));
    connect(legend,
            SIGNAL(itemClicked(size_t)),
            this,
            SLOT(onLegendItemClicked(size_t)));
}

DistributionDiagram::~DistributionDiagram() {}

void DistributionDiagram::setData(const std::vector<DataItem>& data)
{
    selectedSliceIndex = optional<size_t>();
    legend->setData(data);
    diagram->setData(data);
}

void DistributionDiagram::setLegendTitle(const QString& title)
{
    legend->setTitle(title);
}

void DistributionDiagram::setLegendTitleFont(QFont font)
{
    legend->setTitleFont(font);
}

void DistributionDiagram::onChartPartClicked(size_t partIndex)
{
    const bool selectedNewItem
        = selectedSliceIndex && (*selectedSliceIndex != partIndex);
    const bool selectedSameItem = selectedSliceIndex
        && (*selectedSliceIndex == partIndex) && legend->isSelected(partIndex);

    if (selectedNewItem) {
        legend->toggleSelected(*selectedSliceIndex);
        selectedSliceIndex = partIndex;
    }

    selectedSliceIndex = (selectedSameItem) ? optional<size_t>() : partIndex;

    legend->toggleSelected(partIndex);
    diagram->togglePartActive(partIndex);

    emit chartSelectionChanged(partIndex);
}

void DistributionDiagram::onLegendItemClicked(size_t itemIndex)
{
    onChartPartClicked(itemIndex);
}
