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
#ifndef SYNCHRONIZINGACTIONINVOKER_H_KMS4R5HA
#define SYNCHRONIZINGACTIONINVOKER_H_KMS4R5HA

#include "core/ObservableActionInvoker.h"

namespace sprint_timer::compose {

class SyncronizingActionInvoker : public ObservableActionInvoker {
public:
    // TODO(vizier): perhaps as a result of multiple implementations of sync
    // mechanism we have mixed observers
    SyncronizingActionInvoker(ObservableActionInvoker& wrapped_,
                              Observable& desyncObservable_)
        : wrapped{wrapped_}
        , desyncObservable{desyncObservable_}
    {
    }

    void execute(std::unique_ptr<Action> action) override
    {
        wrapped.execute(std::move(action));
        // TODO(vizier): this is technically a second notify, see TODO on top
        desyncObservable.notify();
    }

    void undo() override
    {
        wrapped.undo();
        desyncObservable.notify();
    }

    std::string lastActionDescription() const override
    {
        return wrapped.lastActionDescription();
    }

    bool hasUndoableActions() const override
    {
        return wrapped.hasUndoableActions();
    }

    void attach(Observer& observer) override { wrapped.attach(observer); }

    void detach(Observer& observer) override { wrapped.detach(observer); }

    void notify() override { wrapped.notify(); }

private:
    ObservableActionInvoker& wrapped;
    Observable& desyncObservable;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: SYNCHRONIZINGACTIONINVOKER_H_KMS4R5HA */
