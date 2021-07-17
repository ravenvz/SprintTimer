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
#include "common_utils/FakeUuidGenerator.h"
#include "mocks/StatisticsMediatorMock.h"
#include "qt_gui/presentation/BestWorktimeContract.h"
#include "qt_gui/presentation/BestWorktimePresenter.h"

using sprint_timer::entities::Sprint;
using sprint_timer::ui::BestWorktimePresenter;
using sprint_timer::ui::StatisticsContext;
using sprint_timer::use_cases::SprintDTO;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

namespace colors {

constexpr std::string_view filledColor{"#f63c0d"};

} // namespace colors

namespace sprint_timer::ui::contracts::BestWorktimeContract {

bool operator==(const LegendData& lhs, const LegendData& rhs)
{
    return lhs.periodName == rhs.periodName &&
           lhs.periodHours == rhs.periodHours;
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const LegendData& data)
{
    os << "LegendData{" << data.periodName << ", " << data.periodHours << "}";
    return os;
}

bool operator==(const DiagramData& lhs, const DiagramData& rhs)
{
    return lhs.filledColor == rhs.filledColor &&
           lhs.timeRanges == rhs.timeRanges;
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const DiagramData& data)
{
    os << "DiagramData{\n\t" << data.filledColor << "\n\t";
    for (const auto& element : data.timeRanges) {
        os << dw::to_string(element, "hh.mm.ss dd.MM.yyyy") << ", ";
    }
    os << "\n}";
    return os;
}

} // namespace sprint_timer::ui::contracts::BestWorktimeContract

class DaytimeStatisticsViewMock
    : public sprint_timer::ui::contracts::BestWorktimeContract::View {
public:
    using sprint_timer::ui::contracts::BestWorktimeContract::View::View;

    MOCK_METHOD(
        void,
        updateLegend,
        (const sprint_timer::ui::contracts::BestWorktimeContract::LegendData&),
        (override));
    MOCK_METHOD(
        void,
        updateDiagram,
        (const sprint_timer::ui::contracts::BestWorktimeContract::DiagramData&),
        (override));
};

class BestWorktimePresenterFixture : public ::testing::Test {
public:
    NiceMock<mocks::StatisticsMediatorMock> mediator_mock;
    const size_t numTopTags{20};
    NiceMock<DaytimeStatisticsViewMock> view;
    const dw::DateRange someDateRange{dw::current_date(),
                                      dw::current_date() + dw::Days{1}};
    FakeUuidGenerator generator;

    std::vector<SprintDTO> buildSomeSprints()
    {
        using namespace sprint_timer;
        using namespace std::chrono_literals;
        const std::string taskUuid{"123"};
        const std::string taskName{"Some task"};
        const std::vector<std::string> tags;
        const dw::DateTime dateTime{dw::current_date()};
        const std::vector<dw::DateTimeRange> timeRanges{
            {dateTime + 15h, dateTime + 15h + 25min},
            {dateTime + 15h, dateTime + 15h + 25min},
            {dateTime + 2h, dateTime + 2h + 25min},
            {dateTime + 5h, dateTime + 5h + 25min},
            {dateTime + 3h, dateTime + 3h + 25min}};
        std::vector<SprintDTO> sprints;
        sprints.reserve(timeRanges.size());
        for (const auto& range : timeRanges) {
            sprints.push_back(SprintDTO{
                generator.generateUUID(), taskUuid, taskName, tags, range});
        }
        return sprints;
    }
};

TEST_F(BestWorktimePresenterFixture,
       updates_legend_with_placeholder_data_when_no_date_range_is_supplied)
{
    using sprint_timer::ui::contracts::BestWorktimeContract::LegendData;
    const LegendData expected{"No data", ""};
    const StatisticsContext statisticsContext;
    BestWorktimePresenter sut{mediator_mock, statisticsContext};

    EXPECT_CALL(view, updateLegend(expected));

    sut.attachView(view);
}

TEST_F(BestWorktimePresenterFixture,
       updates_legend_with_placeholder_data_when_distribution_is_empty)
{
    using sprint_timer::ui::contracts::BestWorktimeContract::LegendData;
    const LegendData expected{"No data", ""};
    const StatisticsContext statisticsContext{
        std::vector<SprintDTO>{}, someDateRange, numTopTags};
    BestWorktimePresenter sut{mediator_mock, statisticsContext};

    EXPECT_CALL(view, updateLegend(expected));

    sut.attachView(view);
}

TEST_F(BestWorktimePresenterFixture,
       updates_diagram_with_placeholder_data_when_no_date_range_is_supplied)
{
    using sprint_timer::ui::contracts::BestWorktimeContract::DiagramData;
    const DiagramData expected{std::string{colors::filledColor},
                               std::vector<dw::DateTimeRange>{}};
    const StatisticsContext statisticsContext{
        std::vector<SprintDTO>{}, someDateRange, numTopTags};
    BestWorktimePresenter sut{mediator_mock, statisticsContext};

    EXPECT_CALL(view, updateDiagram(expected));

    sut.attachView(view);
}

TEST_F(BestWorktimePresenterFixture, updates_legend_with_generic_data)
{
    using sprint_timer::ui::contracts::BestWorktimeContract::LegendData;
    using namespace std::chrono_literals;
    const auto sprints = buildSomeSprints();
    const LegendData expected{"Night", "2:00 - 6:00"};
    sprint_timer::ui::StatisticsContext statisticsContext{
        sprints, someDateRange, numTopTags};
    sprint_timer::ui::BestWorktimePresenter sut{mediator_mock,
                                                statisticsContext};

    EXPECT_CALL(view, updateLegend(expected));

    sut.attachView(view);
}

TEST_F(BestWorktimePresenterFixture, updates_diagram_with_generic_data)
{
    using sprint_timer::ui::SprintDaytimeDistribution;
    using sprint_timer::ui::contracts::BestWorktimeContract::DiagramData;
    using namespace std::chrono_literals;
    const sprint_timer::Distribution<double> distribution{
        {0.0, 3.0, 0.0, 2.0, 0.0, 0.0}};
    const dw::DateTime dateTime{dw::current_date()};
    const std::vector<dw::DateTimeRange> timeRanges{
        {dateTime + 15h, dateTime + 15h + 25min},
        {dateTime + 15h, dateTime + 15h + 25min},
        {dateTime + 2h, dateTime + 2h + 25min},
        {dateTime + 5h, dateTime + 5h + 25min},
        {dateTime + 3h, dateTime + 3h + 25min}};
    const SprintDaytimeDistribution daytimeDistribution{distribution,
                                                        timeRanges};
    const auto sprints = buildSomeSprints();
    const DiagramData expected{std::string{colors::filledColor}, timeRanges};
    sprint_timer::ui::StatisticsContext statisticsContext{
        sprints, someDateRange, numTopTags};
    sprint_timer::ui::BestWorktimePresenter sut{mediator_mock,
                                                statisticsContext};

    EXPECT_CALL(view, updateDiagram(expected));

    sut.attachView(view);
}

TEST_F(BestWorktimePresenterFixture, updates_when_shared_data_is_changed)
{
    const dw::DateRange newDateRange{dw::current_date() - dw::Days{-10},
                                     dw::current_date()};
    const std::vector<SprintDTO> stubSprints;
    sprint_timer::ui::StatisticsContext statisticsContext{
        stubSprints, newDateRange, numTopTags};
    sprint_timer::ui::BestWorktimePresenter sut{mediator_mock,
                                                statisticsContext};
    sut.attachView(view);

    EXPECT_CALL(view, updateDiagram(_));
    EXPECT_CALL(view, updateLegend(_));

    sut.onSharedDataChanged();
}
