#ifndef ICOMMAND_H_NHSUJX60
#define ICOMMAND_H_NHSUJX60

#include <string>

class Command {
public:
    virtual ~Command() = default;

    virtual void execute() = 0;

    virtual void undo() {}

    virtual bool supportUndo() const { return false; }

    virtual std::string inspect() const = 0;
};


#endif /* end of include guard: ICOMMAND_H_NHSUJX60 */
