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
#ifndef STATISTICSCONTEXT_H_4GUKJLMO
#define STATISTICSCONTEXT_H_4GUKJLMO

#include "core/AsyncQueryHandler.h"
#include "core/Observable.h"
#include "core/TagTop.h"
#include "core/use_cases/SprintMapper.h"
#include "core/use_cases/request_sprints/RequestSprintsQuery.h"
#include <optional>

#include "core/use_cases/SprintMapper.h"

namespace sprint_timer::ui {

class StatisticsContext {
public:
    StatisticsContext() = default;

    StatisticsContext(const std::vector<use_cases::SprintDTO>& sprints,
                      dw::DateRange dateRange,
                      size_t topTags);

    const std::vector<entities::Sprint>& sprints() const;

    const std::vector<TagTop::TagFrequency>& tagFrequencies() const;

    std::optional<dw::DateRange> currentRange() const;

    void selectTag(std::optional<size_t> tagNumber);

    std::optional<size_t> selectedTag() const;

    size_t numTopTags() const;

private:
    std::vector<entities::Sprint> sprintsTemp;
    std::optional<dw::DateRange> dateRange;
    size_t topTags{5};
    TagTop tagTop;
    std::optional<size_t> tag;
};

inline StatisticsContext::StatisticsContext(
    const std::vector<use_cases::SprintDTO>& sprints_,
    dw::DateRange dateRange_,
    size_t topTags_)
    : sprintsTemp{use_cases::fromDTOs(sprints_)}
    , dateRange{dateRange_}
    , topTags{topTags_}
    , tagTop{TagTop{sprintsTemp, topTags_}}
{
}

inline const std::vector<entities::Sprint>& StatisticsContext::sprints() const
{
    if (tag) {
        return tagTop.sprintsForTagAt(*tag);
    }
    return sprintsTemp;
}

inline const std::vector<TagTop::TagFrequency>&
StatisticsContext::tagFrequencies() const
{
    return tagTop.tagFrequencies();
}

inline std::optional<dw::DateRange> StatisticsContext::currentRange() const
{
    return dateRange;
}

inline void StatisticsContext::selectTag(std::optional<size_t> tagNumber)
{
    tag = tagNumber;
}

inline std::optional<size_t> StatisticsContext::selectedTag() const
{
    return tag;
}

inline size_t StatisticsContext::numTopTags() const { return topTags; }

} // namespace sprint_timer::ui

#endif /* end of include guard: STATISTICSCONTEXT_H_4GUKJLMO */
