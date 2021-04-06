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
#include <core/use_cases/request_tags/AllTagsHandler.h>
#include <qt_gui/presentation/TagEditorPresenter.h>

#include "mocks/CommandHandlerMock.h"
#include "mocks/QueryHandlerMock.h"

using namespace sprint_timer;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Truly;

class TagEditorViewMock
    : public sprint_timer::ui::contracts::TagEditorContract::View {
public:
    MOCK_METHOD(void,
                displayTags,
                (const std::vector<std::string>&),
                (override));
};

class TagEditorPresenterFixture : public ::testing::Test {
public:
    TagEditorViewMock view;
    NiceMock<mocks::QueryHandlerMock<use_cases::AllTagsQuery,
                                     std::vector<std::string>>>
        allTagsHandler;
    NiceMock<mocks::CommandHandlerMock<use_cases::RenameTagCommand>>
        renameTagHandler;
    ui::TagEditorPresenter sut{allTagsHandler, renameTagHandler};
};

TEST_F(TagEditorPresenterFixture, displays_tags_when_view_is_attached)
{
    const std::vector<std::string> tags{"Tag1", "Tag3", "OtherTag"};
    ON_CALL(allTagsHandler, handle(_)).WillByDefault(Return(tags));

    EXPECT_CALL(view, displayTags(tags));

    sut.attachView(view);
}

TEST_F(TagEditorPresenterFixture, invokes_handler_to_rename_tag)
{
    const std::string original{"OldName"};
    const std::string renamed{"Renamed"};
    auto matchesCommand = [&](const use_cases::RenameTagCommand& command) {
        return command.oldName == original && command.newName == renamed;
    };

    EXPECT_CALL(renameTagHandler, handle(Truly(matchesCommand)));

    sut.renameTag(original, renamed);
}
