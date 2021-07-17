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
#include "qt_gui/presentation/UndoPresenter.h"

namespace sprint_timer::ui {

UndoPresenter::UndoPresenter(
    Observable& undoObservable_,
    ActionInvoker& actionInvoker_,
    Mediator<Invalidatable>& cacheInvalidationMediator_)
    : actionInvoker{actionInvoker_}
    , cacheInvalidationMediator{cacheInvalidationMediator_}
    , undoObserver{UndoObserver{undoObservable_, *this}}
{
}

void UndoPresenter::onUndoRequested()
{
    if (auto v = view(); v && actionInvoker.hasUndoableActions()) {
        std::string message{"Revert following action:\n\n"};
        message += actionInvoker.lastActionDescription();
        message.push_back('\n');
        message.push_back('\n');
        message += "Are you sure?";
        v.value()->showConfirmationDialog(message);
    }
}

void UndoPresenter::onUndoConfirmed()
{
    // Note order of calls is important
    // First invalidate all caches
    cacheInvalidationMediator.notifyAll(
        [](auto* colleague) { colleague->invalidate(); });
    // Then allow action invoker to signal resyncronization
    actionInvoker.undo();
}

void UndoPresenter::updateViewImpl()
{
    if (auto v = view(); v) {
        v.value()->setInteractive(actionInvoker.hasUndoableActions());
    }
}

UndoPresenter::UndoObserver::UndoObserver(Observable& undoObservable_,
                                          UndoPresenter& ref_)
    : undoObservable{undoObservable_}
    , ref{ref_}
{
    undoObservable.attach(*this);
}

UndoPresenter::UndoObserver::~UndoObserver() { undoObservable.detach(*this); }

void UndoPresenter::UndoObserver::update() { ref.updateViewImpl(); }

} // namespace sprint_timer::ui
