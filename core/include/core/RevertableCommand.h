#ifndef REVERTABLECOMMANDBASE_H_WWT2GZMK
#define REVERTABLECOMMANDBASE_H_WWT2GZMK

#include "core/Command.h"

class RevertableCommand : public Command {
public:
    virtual ~RevertableCommand() = default;

    virtual void execute() final
    {
        executeAction();
        wasExecuted = true;
    }

    virtual bool supportUndo() const override { return true; }

    virtual void undo() final
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
