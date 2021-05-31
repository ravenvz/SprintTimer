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
#include "core/SprintBuilder.h"
#include "mocks/StatisticsColleagueMock.h"
#include "qt_gui/presentation/TopTagDiagramPresenter.h"
#include "gtest/gtest.h"

using sprint_timer::ui::StatisticsContext;
using sprint_timer::ui::TopTagDiagramPresenter;
using sprint_timer::use_cases::SprintDTO;
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

sprint_timer::ui::StatisticsContext buildFewSprintsContextFixture()
{

    using namespace sprint_timer;
    using namespace sprint_timer::entities;
    dw::DateRange someDateRange{dw::current_date(), dw::current_date()};
    dw::DateTimeRange someTimeSpan{dw::current_date_time(),
                                   dw::current_date_time()};
    const size_t numTopTags{7};
    std::vector<Sprint> sprints;
    SprintBuilder builder;
    builder.withTaskUuid("123").withTimeSpan(someTimeSpan);
    sprints.push_back(builder.withExplicitTags({Tag{"Tag1"}}).build());
    sprints.push_back(builder.withExplicitTags({Tag{"Tag1"}}).build());
    sprints.push_back(builder.withExplicitTags({Tag{"Tag2"}}).build());
    return ui::StatisticsContext{
        use_cases::makeDTOs(sprints), someDateRange, numTopTags};
}

sprint_timer::ui::StatisticsContext
buildLargerSprintsContextFixture(size_t numTopTags)
{
    using namespace sprint_timer;
    using namespace sprint_timer::entities;
    dw::DateRange someDateRange{dw::current_date(), dw::current_date()};
    dw::DateTimeRange someTimeSpan{dw::current_date_time(),
                                   dw::current_date_time()};
    std::vector<Sprint> sprints;
    SprintBuilder builder;
    auto append_n = [&sprints](const auto& sprint, size_t times) {
        for (size_t i = 0; i < times; ++i) {
            sprints.push_back(sprint);
        }
    };
    builder.withTaskUuid("123").withTimeSpan(someTimeSpan);
    append_n(builder.withExplicitTags({Tag{"Tag1"}}).build(), 11);
    append_n(builder.withExplicitTags({Tag{"Tag2"}}).build(), 10);
    append_n(builder.withExplicitTags({Tag{"Tag3"}}).build(), 9);
    append_n(builder.withExplicitTags({Tag{"Tag4"}}).build(), 8);
    append_n(builder.withExplicitTags({Tag{"Tag5"}}).build(), 7);
    append_n(builder.withExplicitTags({Tag{"Tag6"}}).build(), 6);
    append_n(builder.withExplicitTags({Tag{"Tag7"}}).build(), 5);
    append_n(builder.withExplicitTags({Tag{"Tag8"}}).build(), 4);
    append_n(builder.withExplicitTags({Tag{"Tag9"}}).build(), 3);
    append_n(builder.withExplicitTags({Tag{"Tag10"}}).build(), 2);
    append_n(builder.withExplicitTags({Tag{"Tag11"}}).build(), 1);
    return ui::StatisticsContext{
        use_cases::makeDTOs(sprints), someDateRange, numTopTags};
}

} // namespace

namespace sprint_timer::ui::contracts::TopTagDiagramContract {

bool operator==(const DiagramData& lhs, const DiagramData& rhs)
{
    return std::tie(lhs.tagName, lhs.color, lhs.percentage) ==
           std::tie(rhs.tagName, rhs.color, rhs.percentage);
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

    MOCK_METHOD(void, updateDiagram, (std::vector<DiagramData> &&), (override));
    MOCK_METHOD(void,
                updateLegend,
                (const std::vector<std::string>&),
                (override));
    MOCK_METHOD(void, toggleSelection, (std::optional<size_t>), (override));
};

class TagDiagramPresenterFixture : public ::testing::Test {
public:
    sprint_timer::ui::StatisticsMediator statistics_mediator;
    const size_t numTopTags{7};
    NiceMock<TagDiagramview> view;
    NiceMock<mocks::ColleagueMock> fake_colleague;
    dw::DateRange someDateRange{dw::current_date(), dw::current_date()};
    dw::DateTimeRange someTimeSpan{dw::current_date_time(),
                                   dw::current_date_time()};
};

TEST_F(TagDiagramPresenterFixture, does_nothing_when_not_provided_date_range)
{
    StatisticsContext statisticsContext;
    TopTagDiagramPresenter sut{statistics_mediator, statisticsContext};

    EXPECT_CALL(view, updateDiagram(_)).Times(0);

    sut.updateView();
}

TEST_F(TagDiagramPresenterFixture, updates_diagram_with_generic_data)
{
    using ::testing::Truly;
    const std::vector<DiagramData> expected{{"Tag1", colors[0], double{2} / 3},
                                            {"Tag2", colors[1], double{1} / 3}};
    auto has_right_data = [&expected](const std::vector<DiagramData>& arg) {
        return arg == expected;
    };
    const auto statisticsContext = buildFewSprintsContextFixture();
    sprint_timer::ui::TopTagDiagramPresenter sut{statistics_mediator,
                                                 statisticsContext};

    EXPECT_CALL(view, updateDiagram(Truly(has_right_data)));

    sut.attachView(view);
}

TEST_F(TagDiagramPresenterFixture, updates_legend_with_generic_data)
{
    using ::testing::Truly;
    const auto statisticsContext = buildFewSprintsContextFixture();
    TopTagDiagramPresenter sut{statistics_mediator, statisticsContext};
    const std::vector<std::string> expected{"Tag1", "Tag2"};

    EXPECT_CALL(view, updateLegend(expected));

    sut.attachView(view);
}

TEST_F(TagDiagramPresenterFixture, renames_leftover_tags_when_updating_legend)
{

    using ::testing::Truly;
    const size_t maxTags{5};
    const auto statisticsContext = buildLargerSprintsContextFixture(maxTags);
    TopTagDiagramPresenter sut{statistics_mediator, statisticsContext};
    const std::vector<std::string> expected{
        "Tag1", "Tag2", "Tag3", "Tag4", "others"};

    EXPECT_CALL(view, updateLegend(expected));

    sut.attachView(view);
}

TEST_F(TagDiagramPresenterFixture, cycles_through_colors)
{
    using ::testing::Truly;
    const size_t maxTags{12};
    const auto statisticsContext = buildLargerSprintsContextFixture(maxTags);
    TopTagDiagramPresenter sut{statistics_mediator, statisticsContext};
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
    auto has_right_data = [&expected](std::vector<DiagramData>& arg) {
        return arg == expected;
    };

    EXPECT_CALL(view, updateDiagram(Truly(has_right_data)));

    sut.attachView(view);
}

TEST_F(TagDiagramPresenterFixture,
       requeries_handler_when_shared_data_is_changed)
{
    using sprint_timer::SprintBuilder;
    using sprint_timer::entities::Sprint;
    using sprint_timer::entities::Tag;
    using ::testing::Truly;
    dw::DateRange newDateRange{dw::current_date(),
                               dw::current_date() + dw::Days{10}};
    const StatisticsContext statisticsContext{
        std::vector<SprintDTO>{}, newDateRange, numTopTags};
    TopTagDiagramPresenter sut{statistics_mediator, statisticsContext};
    sut.attachView(view);

    EXPECT_CALL(view, updateDiagram(_));
    EXPECT_CALL(view, updateLegend(_));

    sut.onSharedDataChanged();
}

TEST_F(TagDiagramPresenterFixture, changes_to_tag_selection_are_mediated)
{
    const auto statisticsContext = buildLargerSprintsContextFixture(numTopTags);
    TopTagDiagramPresenter sut{statistics_mediator, statisticsContext};
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
    const auto statisticsContext = buildLargerSprintsContextFixture(numTopTags);
    TopTagDiagramPresenter sut{statistics_mediator, statisticsContext};
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
    const auto statisticsContext = buildLargerSprintsContextFixture(numTopTags);
    TopTagDiagramPresenter sut{statistics_mediator, statisticsContext};
    statistics_mediator.addColleague(&fake_colleague);
    sut.attachView(view);
    sut.onTagIndexSelected(2);

    EXPECT_CALL(fake_colleague, onTagSelected(std::optional<size_t>{}));

    sut.onTagIndexSelected(2);
}

TEST_F(TagDiagramPresenterFixture, resets_selection_when_updating_view)
{
    const auto statisticsContext = buildLargerSprintsContextFixture(numTopTags);
    TopTagDiagramPresenter sut{statistics_mediator, statisticsContext};
    const std::optional<size_t> expected;
    sut.onTagIndexSelected(3);
    sut.attachView(view);

    EXPECT_CALL(view, toggleSelection(expected));

    sut.updateView();
}

TEST_F(
    TagDiagramPresenterFixture,
    updates_selection_on_view_when_no_items_were_selected_and_then_some_item_is_selected)
{
    const auto statisticsContext = buildLargerSprintsContextFixture(numTopTags);
    TopTagDiagramPresenter sut{statistics_mediator, statisticsContext};
    const std::optional<size_t> expected{3};
    sut.attachView(view);

    EXPECT_CALL(view, toggleSelection(expected));

    sut.onTagIndexSelected(3);
}

TEST_F(TagDiagramPresenterFixture,
       cancels_selection_on_view_when_same_tag_is_selected_twice)
{
    const auto statisticsContext = buildLargerSprintsContextFixture(numTopTags);
    TopTagDiagramPresenter sut{statistics_mediator, statisticsContext};
    const std::optional<size_t> expected;
    const size_t previouslySelectedIndex{3};
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
    const auto statisticsContext = buildLargerSprintsContextFixture(numTopTags);
    TopTagDiagramPresenter sut{statistics_mediator, statisticsContext};
    statistics_mediator.addColleague(&fake_colleague);
    const std::optional<size_t> expected{5};
    sut.attachView(view);
    sut.onTagIndexSelected(3);

    EXPECT_CALL(view, toggleSelection(std::optional<size_t>(3)));
    EXPECT_CALL(view, toggleSelection(expected));

    sut.onTagIndexSelected(5);
}
