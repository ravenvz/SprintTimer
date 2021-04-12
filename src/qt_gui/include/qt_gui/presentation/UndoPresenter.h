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
#ifndef UNDOPRESENTER_H_UM9IVZNC
#define UNDOPRESENTER_H_UM9IVZNC

#include "qt_gui/presentation/Invalidatable.h"
#include "qt_gui/presentation/Mediator.h"
#include "qt_gui/presentation/UndoContract.h"
#include "core/ActionInvoker.h"
#include "core/Observable.h"
#include "core/Observer.h"

namespace sprint_timer::ui {

class UndoPresenter : public contracts::UndoContract::Presenter {
public:
    friend class UndoObserver;

    UndoPresenter(Observable& undoObservable,
                  ActionInvoker& actionInvoker,
                  Mediator<Invalidatable>& cacheInvalidationMediator);

    void onUndoRequested() override;

    void onUndoConfirmed() override;

private:
    struct UndoObserver : public Observer {
        UndoObserver(Observable& undoObservable, UndoPresenter& ref);

        ~UndoObserver() override;

        void update() override;

    private:
        Observable& undoObservable;
        UndoPresenter& ref;
    };

    ActionInvoker& actionInvoker;

    void updateViewImpl() override;

    void onViewAttached() override;

private:
    Mediator<Invalidatable>& cacheInvalidationMediator;
    UndoObserver undoObserver;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: UNDOPRESENTER_H_UM9IVZNC */
