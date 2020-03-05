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
#include "mocks/StatisticsMediatorMock.h"
#include "gtest/gtest.h"
#include <core/SprintBuilder.h>
#include <qt_gui/presentation/TagPieDiagramPresenter.h>

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using namespace sprint_timer::ui::contracts::TagPieDiagramContract;

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

sprint_timer::TagTop build_2_1_tag_top(const dw::DateTimeRange& someTimeSpan,
                                       size_t numTopTags)
{
    using namespace sprint_timer;
    using namespace sprint_timer::entities;
    std::vector<Sprint> sprints;
    SprintBuilder builder;
    builder.withTaskUuid("123").withTimeSpan(someTimeSpan);
    sprints.push_back(builder.withExplicitTags({Tag{"Tag1"}}).build());
    sprints.push_back(builder.withExplicitTags({Tag{"Tag1"}}).build());
    sprints.push_back(builder.withExplicitTags({Tag{"Tag2"}}).build());
    return TagTop{sprints, numTopTags};
}

sprint_timer::TagTop
build_bigger_tag_top_fixture(const dw::DateTimeRange& someTimeSpan,
                             size_t numTopTags)
{
    using namespace sprint_timer;
    using namespace sprint_timer::entities;
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
    return TagTop{sprints, numTopTags};
}

} // namespace

namespace sprint_timer::ui::contracts::TagPieDiagramContract {

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

} // namespace sprint_timer::ui::contracts::TagPieDiagramContract

class TagDiagramViewMock : public View {
public:
    MOCK_METHOD(void, updateDiagram, (std::vector<DiagramData> &&), (override));
    MOCK_METHOD(void,
                updateLegend,
                (const std::vector<std::string>&),
                (override));
    MOCK_METHOD(void, toggleSelection, (std::optional<size_t>), (override));
};

class TagDiagramPresenterFixture : public ::testing::Test {
public:
    NiceMock<TagDiagramViewMock> viewMock;
    NiceMock<mocks::StatisticsMediatorMock> mediator_mock;
    const size_t numTopTags{7};
    sprint_timer::ui::TagPieDiagramPresenter presenter{mediator_mock};
    dw::DateRange someDateRange{dw::current_date(), dw::current_date()};
    dw::DateTimeRange someTimeSpan{dw::current_date_time(),
                                   dw::current_date_time()};
};

TEST_F(TagDiagramPresenterFixture, does_nothing_when_not_provided_date_range)
{
    // std::vector<sprint_timer::TagTop::TagFrequency> stubFrequencies;
    ON_CALL(mediator_mock, range()).WillByDefault(Return(std::nullopt));
    // ON_CALL(mediator_mock, tagFrequencies())
    //     .WillByDefault(ReturnRef(stubFrequencies));

    EXPECT_CALL(mediator_mock, tagFrequencies()).Times(0);
    EXPECT_CALL(viewMock, updateDiagram(_)).Times(0);

    presenter.attachView(viewMock);
}

TEST_F(TagDiagramPresenterFixture, updates_diagram_with_generic_data)
{
    using ::testing::Truly;
    const auto tagtop = build_2_1_tag_top(someTimeSpan, numTopTags);
    const std::vector<DiagramData> expected{{"Tag1", colors[0], double{2} / 3},
                                            {"Tag2", colors[1], double{1} / 3}};
    auto has_right_data = [&expected](const std::vector<DiagramData>& arg) {
        return arg == expected;
    };
    ON_CALL(mediator_mock, range()).WillByDefault(Return(someDateRange));
    ON_CALL(mediator_mock, tagFrequencies())
        .WillByDefault(ReturnRef(tagtop.tagFrequencies()));

    EXPECT_CALL(viewMock, updateDiagram(Truly(has_right_data)));

    presenter.attachView(viewMock);
}

TEST_F(TagDiagramPresenterFixture, updates_legend_with_generic_data)
{
    using ::testing::Truly;
    const auto tagtop = build_2_1_tag_top(someTimeSpan, numTopTags);
    const std::vector<std::string> expected{"Tag1", "Tag2"};
    ON_CALL(mediator_mock, range()).WillByDefault(Return(someDateRange));
    ON_CALL(mediator_mock, tagFrequencies())
        .WillByDefault(ReturnRef(tagtop.tagFrequencies()));

    EXPECT_CALL(viewMock, updateLegend(expected));

    presenter.attachView(viewMock);
}

TEST_F(TagDiagramPresenterFixture, renames_leftover_tags_when_updating_legend)
{

    using ::testing::Truly;
    const size_t maxTags{5};
    const auto tagtop = build_bigger_tag_top_fixture(someTimeSpan, maxTags);
    const std::vector<std::string> expected{
        "Tag1", "Tag2", "Tag3", "Tag4", "others"};
    ON_CALL(mediator_mock, range()).WillByDefault(Return(someDateRange));
    ON_CALL(mediator_mock, tagFrequencies())
        .WillByDefault(ReturnRef(tagtop.tagFrequencies()));

    EXPECT_CALL(viewMock, updateLegend(expected));

    presenter.attachView(viewMock);
}

TEST_F(TagDiagramPresenterFixture, cycles_through_colors)
{
    using ::testing::Truly;
    const size_t maxTags{12};
    const auto tagtop = build_bigger_tag_top_fixture(someTimeSpan, maxTags);
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
    ON_CALL(mediator_mock, range()).WillByDefault(Return(someDateRange));
    ON_CALL(mediator_mock, tagFrequencies())
        .WillByDefault(ReturnRef(tagtop.tagFrequencies()));

    EXPECT_CALL(viewMock, updateDiagram(Truly(has_right_data)));

    presenter.attachView(viewMock);
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
    presenter.attachView(viewMock);
    const sprint_timer::TagTop stubTagTop;
    ON_CALL(mediator_mock, range()).WillByDefault(Return(someDateRange));

    EXPECT_CALL(mediator_mock, tagFrequencies())
        .WillOnce(ReturnRef(stubTagTop.tagFrequencies()));

    presenter.onSharedDataChanged();
}

TEST_F(TagDiagramPresenterFixture, notifies_mediator_when_tag_is_selected)
{
    const auto tagtop = build_bigger_tag_top_fixture(someTimeSpan, numTopTags);
    std::optional<dw::DateRange> optDateRange{someDateRange};
    ON_CALL(mediator_mock, range())
        .WillByDefault(::testing::Return(optDateRange));
    ON_CALL(mediator_mock, tagFrequencies())
        .WillByDefault(ReturnRef(tagtop.tagFrequencies()));
    presenter.attachView(viewMock);

    EXPECT_CALL(mediator_mock,
                filterByTag(&presenter, std::optional<size_t>(2)));

    presenter.onTagIndexSelected(2);
}

TEST_F(TagDiagramPresenterFixture,
       notifies_mediator_when_tag_is_selected_and_then_another_tag_is_selected)
{
    const auto tagtop = build_bigger_tag_top_fixture(someTimeSpan, numTopTags);
    std::optional<dw::DateRange> optDateRange{someDateRange};
    const std::optional<size_t> selectedTag{2};
    ON_CALL(mediator_mock, range()).WillByDefault(Return(optDateRange));
    ON_CALL(mediator_mock, tagFrequencies())
        .WillByDefault(ReturnRef(tagtop.tagFrequencies()));
    presenter.attachView(viewMock);
    presenter.onTagIndexSelected(3);

    EXPECT_CALL(mediator_mock, filterByTag(&presenter, selectedTag));

    presenter.onTagIndexSelected(2);
}

TEST_F(TagDiagramPresenterFixture,
       notifies_mediator_when_tag_is_selected_and_then_same_tag_is_deselected)
{
    const auto tagtop = build_bigger_tag_top_fixture(someTimeSpan, numTopTags);
    std::optional<dw::DateRange> optDateRange{someDateRange};
    ON_CALL(mediator_mock, range()).WillByDefault(Return(optDateRange));
    ON_CALL(mediator_mock, tagFrequencies())
        .WillByDefault(ReturnRef(tagtop.tagFrequencies()));
    presenter.attachView(viewMock);
    presenter.onTagIndexSelected(2);

    EXPECT_CALL(mediator_mock,
                filterByTag(&presenter, std::optional<size_t>{}));

    presenter.onTagIndexSelected(2);
}

TEST_F(TagDiagramPresenterFixture,
       updates_selection_on_view_when_view_is_attached)
{
    const auto tagtop = build_bigger_tag_top_fixture(someTimeSpan, numTopTags);
    const std::optional<size_t> expected;
    ON_CALL(mediator_mock, range()).WillByDefault(Return(someDateRange));
    ON_CALL(mediator_mock, tagFrequencies())
        .WillByDefault(ReturnRef(tagtop.tagFrequencies()));

    EXPECT_CALL(viewMock, toggleSelection(expected));

    presenter.attachView(viewMock);
}

TEST_F(TagDiagramPresenterFixture,
       updates_selection_on_view_when_view_is_reattached)
{
    const auto tagtop = build_bigger_tag_top_fixture(someTimeSpan, numTopTags);
    const std::optional<size_t> expected;
    ON_CALL(mediator_mock, range()).WillByDefault(Return(someDateRange));
    ON_CALL(mediator_mock, tagFrequencies())
        .WillByDefault(ReturnRef(tagtop.tagFrequencies()));
    presenter.attachView(viewMock);
    presenter.onTagIndexSelected(3);

    EXPECT_CALL(viewMock, toggleSelection(expected));

    presenter.attachView(viewMock);
}

TEST_F(
    TagDiagramPresenterFixture,
    updates_selection_on_view_when_no_items_were_selected_and_then_some_item_is_selected)
{
    const auto tagtop = build_bigger_tag_top_fixture(someTimeSpan, numTopTags);
    const std::optional<size_t> expected{3};
    ON_CALL(mediator_mock, range()).WillByDefault(Return(someDateRange));
    ON_CALL(mediator_mock, tagFrequencies())
        .WillByDefault(ReturnRef(tagtop.tagFrequencies()));
    presenter.attachView(viewMock);

    EXPECT_CALL(viewMock, toggleSelection(expected));

    presenter.onTagIndexSelected(3);
}

TEST_F(TagDiagramPresenterFixture,
       cancels_selection_on_view_when_same_tag_is_selected_twice)
{
    const auto tagtop = build_bigger_tag_top_fixture(someTimeSpan, numTopTags);
    const std::optional<size_t> expected;
    ON_CALL(mediator_mock, range()).WillByDefault(Return(someDateRange));
    ON_CALL(mediator_mock, tagFrequencies())
        .WillByDefault(ReturnRef(tagtop.tagFrequencies()));
    presenter.attachView(viewMock);
    const size_t previouslySelectedIndex{3};
    presenter.onTagIndexSelected(previouslySelectedIndex);

    EXPECT_CALL(
        viewMock,
        toggleSelection(std::optional<size_t>(previouslySelectedIndex)));
    EXPECT_CALL(viewMock, toggleSelection(expected));

    presenter.onTagIndexSelected(previouslySelectedIndex);
}

TEST_F(
    TagDiagramPresenterFixture,
    updates_selection_when_one_item_is_selected_and_then_another_item_is_selected)
{
    const auto tagtop = build_bigger_tag_top_fixture(someTimeSpan, numTopTags);
    const std::optional<size_t> expected{5};
    ON_CALL(mediator_mock, range()).WillByDefault(Return(someDateRange));
    ON_CALL(mediator_mock, tagFrequencies())
        .WillByDefault(ReturnRef(tagtop.tagFrequencies()));
    presenter.attachView(viewMock);
    presenter.onTagIndexSelected(3);

    EXPECT_CALL(viewMock, toggleSelection(std::optional<size_t>(3)));
    EXPECT_CALL(viewMock, toggleSelection(expected));

    presenter.onTagIndexSelected(5);
}
