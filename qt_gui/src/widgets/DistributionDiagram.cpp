/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
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