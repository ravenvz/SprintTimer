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
#include "qt_gui/presentation/TopTagDiagramPresenter.h"

namespace {

const std::vector<std::string> colors{{"#28245a",
                                       "#73c245",
                                       "#ea6136",
                                       "#1d589b",
                                       "#d62a36",
                                       "#401b60",
                                       "#f8cd32",
                                       "#258bc8",
                                       "#087847"}};

// std::vector<sprint_timer::ui::contracts::TopTagDiagramContract::DiagramData>
// toDiagramData(const sprint_timer::TagTop& tagtop);

using ExtractedData = std::pair<
    std::vector<
        sprint_timer::ui::contracts::TopTagDiagramContract::DiagramData>,
    std::vector<std::string>>;

ExtractedData
extractData(const std::vector<sprint_timer::TagTop::TagFrequency>& frequencies);

void renameLeftoverTags(std::vector<std::string>& tagNames);

} // namespace

namespace sprint_timer::ui {

TopTagDiagramPresenter::TopTagDiagramPresenter(
    StatisticsMediator& mediator_, const StatisticsContext& statisticsContext_)
    : mediator{mediator_}
    , statisticsContext{statisticsContext_}
{
    mediator.addColleague(this);
}

TopTagDiagramPresenter::~TopTagDiagramPresenter()
{
    mediator.removeColleague(this);
}

void TopTagDiagramPresenter::onTagIndexSelected(size_t index)
{
    if (auto v = view(); v) {
        // deselect previous selection if any
        if (selection.currentIndex()) {
            v.value()->toggleSelection(selection.currentIndex());
        }
        selection.select(index);
        v.value()->toggleSelection(selection.currentIndex());
        mediator.selectTag(this, selection.currentIndex());
    }
}

void TopTagDiagramPresenter::onSharedDataChanged() { updateView(); }

void TopTagDiagramPresenter::updateViewImpl()
{
    if (auto v = view(); v && statisticsContext.currentRange()) {
        const auto& frequencies = statisticsContext.tagFrequencies();
        auto [diagramData, legendData] = extractData(frequencies);
        selection.setTags(legendData);
        v.value()->toggleSelection(selection.currentIndex());
        v.value()->updateDiagram(std::move(diagramData));
        v.value()->updateLegend(legendData);
    }
}

void TopTagDiagramPresenter::Selection::select(size_t index)
{
    const bool sameIndex{selectedIndex && *selectedIndex == index};
    selectedIndex = sameIndex ? std::nullopt : std::optional<size_t>(index);
}

std::optional<size_t> TopTagDiagramPresenter::Selection::currentIndex() const
{
    return selectedIndex;
}

void TopTagDiagramPresenter::Selection::setTags(
    const std::vector<std::string>& updatedTags)
{
    selectedIndex = std::nullopt;
    tags = updatedTags;
}

} // namespace sprint_timer::ui

namespace {

ExtractedData
extractData(const std::vector<sprint_timer::TagTop::TagFrequency>& frequencies)
{
    using sprint_timer::ui::contracts::TopTagDiagramContract::DiagramData;
    std::vector<DiagramData> diagramData;
    std::vector<std::string> legendData;
    diagramData.reserve(frequencies.size());
    legendData.reserve(frequencies.size());
    for (size_t i = 0; i < frequencies.size(); ++i) {
        diagramData.push_back(DiagramData{frequencies[i].first.name(),
                                          colors[i % colors.size()],
                                          frequencies[i].second});
        legendData.push_back(frequencies[i].first.name());
    }
    renameLeftoverTags(legendData);

    return {diagramData, legendData};
}

void renameLeftoverTags(std::vector<std::string>& tagNames)
{
    if (!tagNames.empty() && tagNames.back().empty())
        tagNames.back() = "others";
}

} // namespace
