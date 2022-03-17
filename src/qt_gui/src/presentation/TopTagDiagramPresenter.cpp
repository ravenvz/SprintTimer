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
#include <array>
#include <ranges>
#include <string_view>

namespace {

constexpr std::array<std::string_view, 9> colors{{"#28245a",
                                                  "#73c245",
                                                  "#ea6136",
                                                  "#1d589b",
                                                  "#d62a36",
                                                  "#401b60",
                                                  "#f8cd32",
                                                  "#258bc8",
                                                  "#087847"}};

struct ExtractedData {
    std::vector<sprint_timer::ui::contracts::TopTagDiagramContract::DiagramData>
        diagramData;
    std::vector<std::string> legendData;
};

ExtractedData
extractData(const sprint_timer::use_cases::TopTagFrequenciesQuery::result_t&
                topTagFrequencies);

void renameLeftoverTags(std::vector<std::string>& tagNames);

} // namespace

namespace sprint_timer::ui {

TopTagDiagramPresenter::TopTagDiagramPresenter(
    top_tag_frequencies_handler_t& topTagFrequenciesHandler_,
    StatisticsMediator& mediator_,
    StatisticsContext& statisticsContext_)
    : topTagFrequenciesHandler{topTagFrequenciesHandler_}
    , mediator{mediator_}
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
        statisticsContext.selectedTag = selection.currentIndex();
        mediator.selectTag(this, selection.currentIndex());
    }
}

void TopTagDiagramPresenter::onSharedDataChanged()
{
    fetchData();
    updateView();
}

void TopTagDiagramPresenter::fetchDataImpl()
{
    const auto [numTopTags, selectedRange, _] = statisticsContext;
    if (selectedRange) {
        topTagFrequencies = topTagFrequenciesHandler.handle(
            use_cases::TopTagFrequenciesQuery{numTopTags, *selectedRange});
    }
}

void TopTagDiagramPresenter::updateViewImpl()
{
    using contracts::TopTagDiagramContract::DiagramData;
    if (auto v = view(); v) {
        const auto [diagramData, legendData] =
            topTagFrequencies ? extractData(topTagFrequencies)
                              : ExtractedData{};
        selection.setTags(legendData);
        v.value()->toggleSelection(selection.currentIndex());
        v.value()->updateDiagram(diagramData);
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
extractData(const sprint_timer::use_cases::TopTagFrequenciesQuery::result_t&
                topTagFrequencies)
{
    using sprint_timer::ui::contracts::TopTagDiagramContract::DiagramData;
    std::vector<DiagramData> diagramData;
    std::vector<std::string> legendData;
    diagramData.reserve(topTagFrequencies->size());
    legendData.reserve(topTagFrequencies->size());

    auto nextColor = [&, i = 0ULL]() mutable {
        return colors[i++ % colors.size()];
    };
    auto fillData = [&](const auto& entry) {
        diagramData.emplace_back(
            entry.tag, std::string{nextColor()}, entry.frequency);
        legendData.emplace_back(entry.tag);
    };
    std::ranges::for_each(*topTagFrequencies, fillData);

    renameLeftoverTags(legendData);

    return {diagramData, legendData};
}

void renameLeftoverTags(std::vector<std::string>& tagNames)
{
    if (!tagNames.empty() && tagNames.back().empty())
        tagNames.back() = "others";
}

} // namespace
