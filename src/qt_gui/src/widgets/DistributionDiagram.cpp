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
#include "qt_gui/widgets/DistributionDiagram.h"
#include "qt_gui/widgets/PieChart.h"
#include "qt_gui/widgets/SimpleLegend.h"
#include <QGridLayout>
#include <QLabel>
#include <QtGui/qpainter.h>
#include <core/entities/Tag.h>
#include <memory>

namespace sprint_timer::ui::qt_gui {

DistributionDiagram::DistributionDiagram(QWidget* parent_)
    : QWidget{parent_}
    , diagram{std::make_unique<PieChart>(this).release()}
    , legend{std::make_unique<SimpleLegend>(this).release()}
{
    auto layout = std::make_unique<QGridLayout>();

    layout->addWidget(std::make_unique<QLabel>("Top Tags").release(), 0, 0);
    layout->addWidget(legend, 1, 0, Qt::AlignTop);
    layout->addWidget(diagram, 0, 1, 20, 5);

    setLayout(layout.release()); // QWidget takes ownership of layout

    connect(diagram,
            &PieChart::partClicked,
            this,
            &DistributionDiagram::onChartPartClicked);
    connect(legend,
            &IStatisticalChartLegend::itemClicked,
            this,
            &DistributionDiagram::onLegendItemClicked);
}

DistributionDiagram::~DistributionDiagram() = default;

void DistributionDiagram::updateLegend(const std::vector<std::string>& tagNames)
{
    legend->setData(tagNames);
}

void DistributionDiagram::updateDiagram(
    std::vector<contracts::TagPieDiagramContract::DiagramData>&& data)
{
    std::vector<PieChart::LabelData> pieChartData;
    pieChartData.reserve(data.size());
    std::transform(cbegin(data),
                   cend(data),
                   std::back_inserter(pieChartData),
                   [](const auto& elem) {
                       return PieChart::LabelData{
                           elem.color, elem.percentage, elem.tagName};
                   });
    diagram->setData(pieChartData);
}

void DistributionDiagram::toggleSelection(std::optional<size_t> selection)
{
    if (!selection)
        return;
    legend->toggleSelected(*selection);
    diagram->togglePartActive(*selection);
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
    if (auto p = presenter(); p) {
        p.value()->onTagIndexSelected(partIndex);
    }
}

void DistributionDiagram::onLegendItemClicked(size_t itemIndex)
{
    onChartPartClicked(itemIndex);
}

} // namespace sprint_timer::ui::qt_gui
