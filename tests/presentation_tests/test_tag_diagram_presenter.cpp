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
#include "mocks/QueryHandlerMock.h"
#include "mocks/StatisticsColleagueMock.h"
#include "qt_gui/presentation/TopTagDiagramPresenter.h"
#include "gtest/gtest.h"

using sprint_timer::ui::StatisticsContext;
using sprint_timer::ui::TopTagDiagramPresenter;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using namespace sprint_timer::ui::contracts::TopTagDiagramContract;

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

} // namespace

namespace sprint_timer::ui::contracts::TopTagDiagramContract {

bool operator==(const DiagramData& lhs, const DiagramData& rhs)
{
    return lhs.tagName == rhs.tagName and lhs.color == rhs.color and
           std::abs(lhs.percentage - rhs.percentage) < 0.01;
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const DiagramData& data)
{
    os << "DiagramData{" << data.tagName << ", " << data.color << ", "
       << data.percentage << "}";
    return os;
}

} // namespace sprint_timer::ui::contracts::TopTagDiagramContract

class TagDiagramview : public View {
public:
    using View::View;

    MOCK_METHOD(void,
                updateDiagram,
                (std::span<const DiagramData>),
                (override));
    MOCK_METHOD(void,
                updateLegend,
                (const std::vector<std::string>&),
                (override));
    MOCK_METHOD(void, toggleSelection, (std::optional<size_t>), (override));
};

class TagDiagramPresenterFixture : public ::testing::Test {
public:
    sprint_timer::ui::StatisticsMediator statistics_mediator;
    const size_t numTopTags{4};
    NiceMock<TagDiagramview> view;
    NiceMock<mocks::ColleagueMock> fake_colleague;
    NiceMock<mocks::QueryHandlerMock<
        sprint_timer::use_cases::TopTagFrequenciesQuery>>
        topTagFrequenciesHandler;
    dw::DateRange someDateRange{dw::current_date(), dw::current_date()};
    dw::DateTimeRange someTimeSpan{dw::current_date_time(),
                                   dw::current_date_time()};
};

TEST_F(TagDiagramPresenterFixture, does_nothing_when_not_provided_date_range)
{
    StatisticsContext statisticsContext;
    TopTagDiagramPresenter sut{
        topTagFrequenciesHandler, statistics_mediator, statisticsContext};

    EXPECT_CALL(view, updateDiagram(_)).Times(0);

    sut.updateView();
}

TEST_F(TagDiagramPresenterFixture, updates_diagram_with_generic_data)
{
    using ::testing::Truly;
    const std::vector<DiagramData> expected{{"Tag1", colors[0], double{2} / 3},
                                            {"Tag2", colors[1], double{1} / 3}};
    auto has_right_data = [&expected](std::span<const DiagramData> arg) {
        return std::ranges::equal(expected, arg);
    };
    const StatisticsContext statisticsContext{someDateRange};
    sprint_timer::ui::TopTagDiagramPresenter sut{
        topTagFrequenciesHandler, statistics_mediator, statisticsContext};
    mocks::given_handler_returns(
        topTagFrequenciesHandler,
        std::vector<sprint_timer::use_cases::TagFrequencyDTO>{
            {double{2} / 3, "Tag1"}, {double{1} / 3, "Tag2"}});

    EXPECT_CALL(view, updateDiagram(Truly(has_right_data)));

    sut.attachView(view);
}

TEST_F(TagDiagramPresenterFixture, updates_legend_with_generic_data)
{
    using ::testing::Truly;
    const StatisticsContext statisticsContext{someDateRange};
    TopTagDiagramPresenter sut{
        topTagFrequenciesHandler, statistics_mediator, statisticsContext};
    const std::vector<std::string> expected{"Tag1", "Tag2"};
    mocks::given_handler_returns(
        topTagFrequenciesHandler,
        std::vector<sprint_timer::use_cases::TagFrequencyDTO>{
            {double{2} / 3, "Tag1"}, {double{1} / 3, "Tag2"}});

    EXPECT_CALL(view, updateLegend(expected));

    sut.attachView(view);
}

TEST_F(TagDiagramPresenterFixture, renames_leftover_tags_when_updating_legend)
{

    using ::testing::Truly;
    const StatisticsContext statisticsContext{someDateRange};
    TopTagDiagramPresenter sut{
        topTagFrequenciesHandler, statistics_mediator, statisticsContext};
    const std::vector<std::string> expected{
        "Tag1", "Tag2", "Tag3", "Tag4", "others"};
    mocks::given_handler_returns(
        topTagFrequenciesHandler,
        std::vector<sprint_timer::use_cases::TagFrequencyDTO>{
            {0, "Tag1"}, {0, "Tag2"}, {0, "Tag3"}, {0, "Tag4"}, {0, ""}});

    EXPECT_CALL(view, updateLegend(expected));

    sut.attachView(view);
}

TEST_F(TagDiagramPresenterFixture, cycles_through_colors)
{
    using ::testing::Truly;
    const StatisticsContext statisticsContext{someDateRange};
    TopTagDiagramPresenter sut{
        topTagFrequenciesHandler, statistics_mediator, statisticsContext};
    const std::vector<DiagramData> expected{
        {"Tag1", colors[0], double{11} / 66},
        {"Tag2", colors[1], double{10} / 66},
        {"Tag3", colors[2], double{9} / 66},
        {"Tag4", colors[3], double{8} / 66},
        {"Tag5", colors[4], double{7} / 66},
        {"Tag6", colors[5], double{6} / 66},
        {"Tag7", colors[6], double{5} / 66},
        {"Tag8", colors[7], double{4} / 66},
        {"Tag9", colors[8], double{3} / 66},
        {"Tag10", colors[0], double{2} / 66},
        {"Tag11", colors[1], double{1} / 66},
    };
    auto has_right_data = [&expected](std::span<const DiagramData> arg) {
        return std::ranges::equal(expected, arg);
    };
    mocks::given_handler_returns(
        topTagFrequenciesHandler,
        std::vector<sprint_timer::use_cases::TagFrequencyDTO>{
            {double{11} / 66, "Tag1"},
            {double{10} / 66, "Tag2"},
            {double{9} / 66, "Tag3"},
            {double{8} / 66, "Tag4"},
            {double{7} / 66, "Tag5"},
            {double{6} / 66, "Tag6"},
            {double{5} / 66, "Tag7"},
            {double{4} / 66, "Tag8"},
            {double{3} / 66, "Tag9"},
            {double{2} / 66, "Tag10"},
            {double{1} / 66, "Tag11"}});

    EXPECT_CALL(view, updateDiagram(Truly(has_right_data)));

    sut.attachView(view);
}

TEST_F(TagDiagramPresenterFixture, updates_diagram_when_shared_data_is_changed)
{
    using ::testing::Truly;
    const std::vector<DiagramData> expected{{"Tag1", colors[0], double{2} / 3},
                                            {"Tag2", colors[1], double{1} / 3}};
    auto has_right_data = [&expected](std::span<const DiagramData> arg) {
        return std::ranges::equal(expected, arg);
    };
    const StatisticsContext statisticsContext{someDateRange};
    sprint_timer::ui::TopTagDiagramPresenter sut{
        topTagFrequenciesHandler, statistics_mediator, statisticsContext};
    sut.attachView(view);
    mocks::given_handler_returns(
        topTagFrequenciesHandler,
        std::vector<sprint_timer::use_cases::TagFrequencyDTO>{
            {double{2} / 3, "Tag1"}, {double{1} / 3, "Tag2"}});

    EXPECT_CALL(view, updateDiagram(Truly(has_right_data)));

    sut.onSharedDataChanged();
}

TEST_F(TagDiagramPresenterFixture, updates_legend_when_shared_data_is_changed)
{
    using ::testing::Truly;
    const StatisticsContext statisticsContext{someDateRange};
    TopTagDiagramPresenter sut{
        topTagFrequenciesHandler, statistics_mediator, statisticsContext};
    sut.attachView(view);
    const std::vector<std::string> expected{"Tag1", "Tag2"};
    mocks::given_handler_returns(
        topTagFrequenciesHandler,
        std::vector<sprint_timer::use_cases::TagFrequencyDTO>{
            {double{2} / 3, "Tag1"}, {double{1} / 3, "Tag2"}});

    EXPECT_CALL(view, updateLegend(expected));

    sut.onSharedDataChanged();
}

TEST_F(TagDiagramPresenterFixture, changes_to_tag_selection_are_mediated)
{
    const StatisticsContext statisticsContext{someDateRange};
    TopTagDiagramPresenter sut{
        topTagFrequenciesHandler, statistics_mediator, statisticsContext};
    statistics_mediator.addColleague(&fake_colleague);
    std::optional<size_t> selectedTag{2};
    sut.attachView(view);

    EXPECT_CALL(fake_colleague, onTagSelected(selectedTag));

    sut.onTagIndexSelected(2);
}

TEST_F(
    TagDiagramPresenterFixture,
    changes_to_tag_selection_are_mediated_when_another_tag_is_selected_after_attaching_view)
{
    const StatisticsContext statisticsContext{someDateRange};
    TopTagDiagramPresenter sut{
        topTagFrequenciesHandler, statistics_mediator, statisticsContext};
    statistics_mediator.addColleague(&fake_colleague);
    const std::optional<size_t> selectedTag{2};
    sut.attachView(view);
    sut.onTagIndexSelected(3);

    EXPECT_CALL(fake_colleague, onTagSelected(selectedTag));

    sut.onTagIndexSelected(2);
}

TEST_F(TagDiagramPresenterFixture,
       changes_are_mediated_when_tag_is_selected_and_then_deselected)
{
    const StatisticsContext statisticsContext{someDateRange};
    TopTagDiagramPresenter sut{
        topTagFrequenciesHandler, statistics_mediator, statisticsContext};
    statistics_mediator.addColleague(&fake_colleague);
    sut.attachView(view);
    sut.onTagIndexSelected(2);

    EXPECT_CALL(fake_colleague, onTagSelected(std::optional<size_t>{}));

    sut.onTagIndexSelected(2);
}

TEST_F(TagDiagramPresenterFixture, resets_selection_when_updating_view)
{
    const StatisticsContext statisticsContext{someDateRange};
    TopTagDiagramPresenter sut{
        topTagFrequenciesHandler, statistics_mediator, statisticsContext};
    const std::optional<size_t> expected;
    sut.onTagIndexSelected(3);
    const std::vector<sprint_timer::use_cases::TagFrequencyDTO> irrelevantData;
    mocks::given_handler_returns(topTagFrequenciesHandler, irrelevantData);
    sut.attachView(view);

    EXPECT_CALL(view, toggleSelection(expected));

    sut.updateView();
}

TEST_F(
    TagDiagramPresenterFixture,
    updates_selection_on_view_when_no_items_were_selected_and_then_some_item_is_selected)
{
    const StatisticsContext statisticsContext{someDateRange};
    TopTagDiagramPresenter sut{
        topTagFrequenciesHandler, statistics_mediator, statisticsContext};
    const std::vector<sprint_timer::use_cases::TagFrequencyDTO> irrelevantData;
    mocks::given_handler_returns(topTagFrequenciesHandler, irrelevantData);
    const std::optional<size_t> expected{3};
    sut.attachView(view);

    EXPECT_CALL(view, toggleSelection(expected));

    sut.onTagIndexSelected(3);
}

TEST_F(TagDiagramPresenterFixture,
       cancels_selection_on_view_when_same_tag_is_selected_twice)
{
    const StatisticsContext statisticsContext{someDateRange};
    TopTagDiagramPresenter sut{
        topTagFrequenciesHandler, statistics_mediator, statisticsContext};
    const std::optional<size_t> expected;
    const size_t previouslySelectedIndex{3};
    const std::vector<sprint_timer::use_cases::TagFrequencyDTO> irrelevantData;
    mocks::given_handler_returns(topTagFrequenciesHandler, irrelevantData);
    sut.attachView(view);
    sut.onTagIndexSelected(previouslySelectedIndex);

    EXPECT_CALL(
        view, toggleSelection(std::optional<size_t>(previouslySelectedIndex)));
    EXPECT_CALL(view, toggleSelection(expected));

    sut.onTagIndexSelected(previouslySelectedIndex);
}

TEST_F(
    TagDiagramPresenterFixture,
    updates_selection_when_one_item_is_selected_and_then_another_item_is_selected)
{
    const StatisticsContext statisticsContext{someDateRange};
    TopTagDiagramPresenter sut{
        topTagFrequenciesHandler, statistics_mediator, statisticsContext};
    statistics_mediator.addColleague(&fake_colleague);
    const std::optional<size_t> expected{5};
    const std::vector<sprint_timer::use_cases::TagFrequencyDTO> irrelevantData;
    mocks::given_handler_returns(topTagFrequenciesHandler, irrelevantData);
    sut.attachView(view);
    sut.onTagIndexSelected(3);

    EXPECT_CALL(view, toggleSelection(std::optional<size_t>(3)));
    EXPECT_CALL(view, toggleSelection(expected));

    sut.onTagIndexSelected(5);
}
