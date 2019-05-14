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
#include "qt_gui/widgets/DistributionDiagram.h"
#include <QHBoxLayout>
#include <QtGui/qpainter.h>
#include <core/entities/Tag.h>
#include <memory>

namespace {

void nameMergedTags(
    std::vector<sprint_timer::TagTop::TagFrequency>& tagFrequencies);

std::vector<sprint_timer::ui::qt_gui::DataItem> toDataItems(
    const std::vector<sprint_timer::TagTop::TagFrequency>& tagFrequencies);

} // namespace

namespace sprint_timer::ui::qt_gui {

DistributionDiagram::DistributionDiagram(QWidget* parent)
    : QWidget{parent}
{
    auto layout = std::make_unique<QHBoxLayout>();
    diagram = std::make_unique<PieChart>(this).release();
    legend = std::make_unique<SimpleLegend>(this).release();
    layout->addWidget(legend);
    layout->addWidget(diagram);
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

void DistributionDiagram::setData(
    std::vector<TagTop::TagFrequency>&& tagFrequencies)
{
    selectedSliceIndex = std::optional<size_t>();
    nameMergedTags(tagFrequencies);
    const auto data = toDataItems(tagFrequencies);
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

    selectedSliceIndex
        = (selectedSameItem) ? std::optional<size_t>() : partIndex;

    legend->toggleSelected(partIndex);
    diagram->togglePartActive(partIndex);

    emit chartSelectionChanged(partIndex);
}

void DistributionDiagram::onLegendItemClicked(size_t itemIndex)
{
    onChartPartClicked(itemIndex);
}

} // namespace sprint_timer::ui::qt_gui


namespace {

void nameMergedTags(
    std::vector<sprint_timer::TagTop::TagFrequency>& tagFrequencies)
{
    if (!tagFrequencies.empty()
        && tagFrequencies.back().first == sprint_timer::entities::Tag{""})
        tagFrequencies.back().first.setName("others");
}

std::vector<sprint_timer::ui::qt_gui::DataItem> toDataItems(
    const std::vector<sprint_timer::TagTop::TagFrequency>& tagFrequencies)
{
    std::vector<std::pair<std::string, double>> data;
    data.reserve(tagFrequencies.size());
    std::transform(tagFrequencies.cbegin(),
                   tagFrequencies.cend(),
                   std::back_inserter(data),
                   [](const auto& elem) -> std::pair<std::string, double> {
                       return {elem.first.name(), elem.second};
                   });
    return data;
}

} // namespace
