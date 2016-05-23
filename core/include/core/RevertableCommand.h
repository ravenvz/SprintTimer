#ifndef REVERTABLECOMMANDBASE_H_WWT2GZMK
#define REVERTABLECOMMANDBASE_H_WWT2GZMK


class RevertableCommand {
public:
    virtual ~RevertableCommand() = default;

    virtual void execute()
    {
        executeAction();
        wasExecuted = true;
    }

    virtual void undo()
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
