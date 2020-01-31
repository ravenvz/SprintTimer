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
#include "core/ProgressOverPeriod.h"
#include <tuple>

namespace {

std::tuple<int, int, int>
unpackData(const std::vector<sprint_timer::GoalProgress>& data);

} // namespace

namespace sprint_timer {

ProgressOverPeriod::ProgressOverPeriod(std::vector<GoalProgress>&& data_)
    : data{std::move(data_)}
{
    const auto unpacked = unpackData(data);
    std::tie(totalActual, totalEstimated, numBins) = unpacked;
}

std::optional<double> ProgressOverPeriod::percentage() const
{
    return numBins == 0 ? std::optional<double>{}
                        : static_cast<double>(actual()) * 100 / estimated();
}

int ProgressOverPeriod::estimated() const { return totalEstimated; }

int ProgressOverPeriod::actual() const { return totalActual; }

std::optional<double> ProgressOverPeriod::averagePerGroupPeriod() const
{
    return numBins == 0 ? std::optional<double>{}
                        : static_cast<double>(actual()) / numBins;
}

GoalProgress ProgressOverPeriod::getValue(size_t binNumber) const
{
    return data[binNumber];
}

size_t ProgressOverPeriod::size() const { return data.size(); }

bool ProgressOverPeriod::isOverwork() const { return difference() < 0; }

int ProgressOverPeriod::difference() const { return estimated() - actual(); }

} // namespace sprint_timer

namespace {

std::tuple<int, int, int>
unpackData(const std::vector<sprint_timer::GoalProgress>& data)
{
    std::tuple<int, int, int> res{0, 0, 0};
    for (const auto& element : data) {
        std::get<0>(res) += element.actual();
        std::get<1>(res) += element.estimated();
        std::get<2>(res) += (element.estimated() ? 1 : 0);
    }
    return res;
}

} // namespace
