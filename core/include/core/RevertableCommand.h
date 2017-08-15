/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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
#ifndef REVERTABLECOMMANDBASE_H_WWT2GZMK
#define REVERTABLECOMMANDBASE_H_WWT2GZMK

#include "core/Command.h"

class RevertableCommand : public Command {
public:

    void execute() final
    {
        executeAction();
        wasExecuted = true;
    }

    bool supportUndo() const override { return true; }

    void undo() final
    {
        if (wasExecuted) {
            undoAction();
            wasExecuted = false;
        }
    }

protected:
    virtual void executeAction() = 0;

    virtual void undoAction() = 0;

private:
    bool wasExecuted{false};
};


#endif /* end of include guard: REVERTABLECOMMANDBASE_H_WWT2GZMK */
