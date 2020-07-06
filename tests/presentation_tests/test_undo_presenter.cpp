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
#include "gmock/gmock.h"
#include <qt_gui/presentation/UndoContract.h>
#include <qt_gui/presentation/UndoPresenter.h>

using namespace ::testing;

class UndoViewMock : public sprint_timer::ui::contracts::UndoContract::View {
public:
    MOCK_METHOD(void, showConfirmationDialog, (const std::string&), (override));

    MOCK_METHOD(void, setInteractive, (bool), (override));
};

class ActionInvokerMock : public sprint_timer::ActionInvoker {
public:
    MOCK_METHOD(void,
                execute,
                (std::unique_ptr<sprint_timer::Action>),
                (override));

    MOCK_METHOD(void, undo, (), (override));

    MOCK_METHOD(std::string, lastActionDescription, (), (const override));

    MOCK_METHOD(bool, hasUndoableActions, (), (const override));
};

class UndoPresenterFixture : public ::testing::Test {
public:
    NiceMock<UndoViewMock> view;
    NiceMock<ActionInvokerMock> actionInvoker;
    sprint_timer::Observable observable;
    sprint_timer::ui::UndoPresenter presenter{observable, actionInvoker};
};

TEST_F(
    UndoPresenterFixture,
    makes_view_interactive_when_view_is_attached_and_there_are_undoable_actions)
{
    ON_CALL(actionInvoker, hasUndoableActions()).WillByDefault(Return(false));

    EXPECT_CALL(view, setInteractive(false));

    presenter.attachView(view);
}

TEST_F(
    UndoPresenterFixture,
    makes_view_uninteractive_when_view_is_attached_and_there_are_no_undoable_actions)
{
    ON_CALL(actionInvoker, hasUndoableActions()).WillByDefault(Return(true));

    EXPECT_CALL(view, setInteractive(true));

    presenter.attachView(view);
}

TEST_F(UndoPresenterFixture, makes_view_show_undo_dialog_when_undo_requested)
{
    const std::string expectedMessage{
        "Revert following action:\n\nmessage_from_invoker\n\nAre you sure?"};
    ON_CALL(actionInvoker, hasUndoableActions()).WillByDefault(Return(true));
    ON_CALL(actionInvoker, lastActionDescription())
        .WillByDefault(Return(std::string{"message_from_invoker"}));
    presenter.attachView(view);

    EXPECT_CALL(view, showConfirmationDialog(expectedMessage));

    presenter.onUndoRequested();
}

TEST_F(
    UndoPresenterFixture,
    does_nothing_when_undo_requested_but_action_invoker_has_no_undoable_actions)
{
    ON_CALL(actionInvoker, hasUndoableActions()).WillByDefault(Return(false));
    ON_CALL(actionInvoker, lastActionDescription())
        .WillByDefault(Return(std::string{"message_from_invoker"}));
    presenter.attachView(view);

    EXPECT_CALL(view, showConfirmationDialog(_)).Times(0);

    presenter.onUndoRequested();
}

TEST_F(
    UndoPresenterFixture,
    makes_view_uninteractive_when_there_are_no_undoable_actions_and_observable_changed)
{
    ON_CALL(actionInvoker, hasUndoableActions()).WillByDefault(Return(false));
    presenter.attachView(view);

    EXPECT_CALL(view, setInteractive(false));

    observable.notify();
}

TEST_F(
    UndoPresenterFixture,
    makes_view_interactive_when_there_are_undoable_actions_and_observable_changed)
{
    ON_CALL(actionInvoker, hasUndoableActions()).WillByDefault(Return(true));
    presenter.attachView(view);

    EXPECT_CALL(view, setInteractive(true));

    observable.notify();
}

TEST_F(UndoPresenterFixture, forwards_undo_call_when_undo_confirmed)
{
    presenter.attachView(view);

    EXPECT_CALL(actionInvoker, undo());

    presenter.onUndoConfirmed();
}
