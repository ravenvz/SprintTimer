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
#include "core/TagTop.h"
#include "core/utils/Algutils.h"
#include <numeric>
#include <ranges>

namespace sprint_timer {

TagTop::TagTop(const std::vector<SprintRecord>& sprints_, size_t topMaxSize_)
    : numTopTags{topMaxSize_}
{
    arrangeSprintsByTag(sprints_);
    computeTagFrequencies();
    buildTagTop();
}

TagTop::TagTop(std::span<const sprint_tags_t> input_, size_t topSize_)
    : numTopTags{topSize_}
{
    for (const auto& [timeRange, tags] : input_) {
        dateRanges.emplace_back(timeRange);
        for (const auto& tag : tags) {
            sprintsByTag[tag].push_back(timeRange);
        }
    }
    computeTagFrequencies();
    buildTagTop();
}

void TagTop::arrangeSprintsByTag(const std::vector<SprintRecord>& sprints)
{
    for (const SprintRecord& sprint : sprints) {
        dateRanges.emplace_back(sprint.timeSpan());
        for (const auto& tag : sprint.tags()) {
            sprintsByTag[tag].push_back(sprint.timeSpan());
        }
    }
}

void TagTop::computeTagFrequencies()
{
    const auto total =
        ranges_ext::fold(sprintsByTag, 0ULL, [](auto acc, const auto& entry) {
            return acc + entry.second.size();
        });
    auto tag_frequency = [total = static_cast<double>(total)](
                             const auto& entry) -> TagFrequency {
        const auto& [tag, intervals] = entry;
        return {tag, static_cast<double>(intervals.size()) / total};
    };
    // clang-format off
    const auto frequencies_view = sprintsByTag
                                    | std::views::transform(tag_frequency) 
                                    | std::views::common;
    // clang-format on
    frequencies = std::vector<TagFrequency>(frequencies_view.begin(),
                                            frequencies_view.end());
}

void TagTop::buildTagTop()
{
    const int64_t limit = static_cast<int64_t>(
        std::max(std::min(numTopTags, frequencies.size()), 1UL) - 1);
    std::ranges::partial_sort(frequencies,
                              begin(frequencies) + limit,
                              std::greater<double>{},
                              [](const auto& entry) { return entry.second; });

    if (frequencies.size() < numTopTags) {
        return;
    }

    const auto bottomTags =
        std::ranges::subrange(begin(frequencies) + limit, end(frequencies));

    const auto combinedBottomTagFrequency = ranges_ext::fold(
        bottomTags | std::views::elements<1>, 0.0, std::plus<double>{});

    const auto bottomTagsView = bottomTags | std::views::elements<0>;

    std::ranges::for_each(bottomTagsView, [this](const auto& tag) {
        std::ranges::copy(sprintsByTag[tag],
                          std::back_inserter(sprintsByTag[dummyTag]));
    });
    std::ranges::sort(
        sprintsByTag[dummyTag], std::less{}, &dw::DateTimeRange::start);

    frequencies.erase(begin(frequencies) + limit + 1, end(frequencies));
    frequencies.back() = {dummyTag, combinedBottomTagFrequency};
}

const std::vector<TagTop::TagFrequency>& TagTop::tagFrequencies() const
{
    return frequencies;
}

const std::vector<dw::DateTimeRange>&
TagTop::sprintsForTagAt(std::optional<size_t> position) const
{
    if (!position) {
        return dateRanges;
    }
    return sprintsByTag.at(Tag{tagNameAt(*position)});
}

std::string TagTop::tagNameAt(size_t position) const
{
    return frequencies.at(position).first.name();
}

size_t TagTop::topSize() const { return frequencies.size(); }

} // namespace sprint_timer
