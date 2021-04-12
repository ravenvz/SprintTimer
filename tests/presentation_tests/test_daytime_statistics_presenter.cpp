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
#include "mocks/StatisticsMediatorMock.h"
#include <core/SprintBuilder.h>
#include <qt_gui/presentation/DaytimeStatisticsContract.h>
#include <qt_gui/presentation/DaytimeStatisticsPresenter.h>

using sprint_timer::entities::Sprint;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

namespace colors {

constexpr std::string_view filledColor{"#f63c0d"};

} // namespace colors

namespace sprint_timer::ui::contracts::DaytimeStatisticsContract {

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

} // namespace sprint_timer::ui::contracts::DaytimeStatisticsContract

class DaytimeStatisticsViewMock
    : public sprint_timer::ui::contracts::DaytimeStatisticsContract::View {
public:
    using sprint_timer::ui::contracts::DaytimeStatisticsContract::View::View;

    MOCK_METHOD(void,
                updateLegend,
                (const sprint_timer::ui::contracts::DaytimeStatisticsContract::
                     LegendData&),
                (override));
    MOCK_METHOD(void,
                updateDiagram,
                (const sprint_timer::ui::contracts::DaytimeStatisticsContract::
                     DiagramData&),
                (override));
};

class DaytimeStatisticsPresenterFixture : public ::testing::Test {
public:
    NiceMock<mocks::StatisticsMediatorMock> mediator_mock;
    const size_t numTopTags{20};
    sprint_timer::ui::DaytimeStatisticsPresenter sut{mediator_mock};
    NiceMock<DaytimeStatisticsViewMock> view;
    const dw::DateRange someDateRange{dw::current_date(),
                                      dw::current_date() + dw::Days{1}};

    std::vector<Sprint> buildSprintsFixture()
    {
        using namespace sprint_timer;
        using namespace std::chrono_literals;
        std::vector<Sprint> sprints;
        SprintBuilder builder;
        builder.withTaskUuid("123");
        const dw::DateTime dateTime{dw::current_date()};
        const std::vector<dw::DateTimeRange> timeRanges{
            {dateTime + 15h, dateTime + 15h + 25min},
            {dateTime + 15h, dateTime + 15h + 25min},
            {dateTime + 2h, dateTime + 2h + 25min},
            {dateTime + 5h, dateTime + 5h + 25min},
            {dateTime + 3h, dateTime + 3h + 25min}};
        for (const auto& range : timeRanges)
            sprints.push_back(builder.withTimeSpan(range).build());
        return sprints;
    }
};

TEST_F(DaytimeStatisticsPresenterFixture,
       updates_legend_with_placeholder_data_when_no_date_range_is_supplied)
{
    using sprint_timer::ui::contracts::DaytimeStatisticsContract::LegendData;
    const LegendData expected{"No data", ""};
    const std::optional<dw::DateRange> emptyRange;
    ON_CALL(mediator_mock, range()).WillByDefault(Return(emptyRange));

    EXPECT_CALL(view, updateLegend(expected));

    sut.attachView(view);
}

TEST_F(DaytimeStatisticsPresenterFixture,
       updates_legend_with_placeholder_data_when_distribution_is_empty)
{
    using sprint_timer::ui::contracts::DaytimeStatisticsContract::LegendData;
    const LegendData expected{"No data", ""};
    const std::vector<Sprint> sprints;
    ON_CALL(mediator_mock, range()).WillByDefault(Return(someDateRange));
    ON_CALL(mediator_mock, sprints()).WillByDefault(ReturnRef(sprints));

    EXPECT_CALL(view, updateLegend(expected));

    sut.attachView(view);
}

TEST_F(DaytimeStatisticsPresenterFixture,
       updates_diagram_with_placeholder_data_when_no_date_range_is_supplied)
{
    using sprint_timer::ui::contracts::DaytimeStatisticsContract::DiagramData;
    const DiagramData expected{std::string{colors::filledColor},
                               std::vector<dw::DateTimeRange>{}};
    const std::vector<Sprint> sprints;
    ON_CALL(mediator_mock, range()).WillByDefault(Return(someDateRange));
    ON_CALL(mediator_mock, sprints()).WillByDefault(ReturnRef(sprints));

    EXPECT_CALL(view, updateDiagram(expected));

    sut.attachView(view);
}

TEST_F(DaytimeStatisticsPresenterFixture, updates_legend_with_generic_data)
{
    using sprint_timer::ui::contracts::DaytimeStatisticsContract::LegendData;
    using namespace std::chrono_literals;
    const auto sprints = buildSprintsFixture();
    const LegendData expected{"Night", "2:00 - 6:00"};
    ON_CALL(mediator_mock, range()).WillByDefault(Return(someDateRange));
    ON_CALL(mediator_mock, sprints()).WillByDefault(ReturnRef(sprints));

    EXPECT_CALL(view, updateLegend(expected));

    sut.attachView(view);
}

TEST_F(DaytimeStatisticsPresenterFixture, updates_diagram_with_generic_data)
{
    using sprint_timer::ui::SprintDaytimeDistribution;
    using sprint_timer::ui::contracts::DaytimeStatisticsContract::DiagramData;
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
    const auto sprints = buildSprintsFixture();
    const DiagramData expected{std::string{colors::filledColor}, timeRanges};
    ON_CALL(mediator_mock, range()).WillByDefault(Return(someDateRange));
    ON_CALL(mediator_mock, sprints()).WillByDefault(ReturnRef(sprints));

    EXPECT_CALL(view, updateDiagram(expected));

    sut.attachView(view);
}

TEST_F(DaytimeStatisticsPresenterFixture,
       requeries_mediator_when_shared_data_is_changed)
{
    const dw::DateRange newDateRange{dw::current_date() - dw::Days{-10},
                                     dw::current_date()};
    const std::vector<Sprint> stubSprints;
    ON_CALL(mediator_mock, range()).WillByDefault(Return(newDateRange));
    ON_CALL(mediator_mock, sprints()).WillByDefault(ReturnRef(stubSprints));
    sut.attachView(view);

    EXPECT_CALL(view, updateDiagram(_));
    EXPECT_CALL(view, updateLegend(_));

    sut.onSharedDataChanged();
}
