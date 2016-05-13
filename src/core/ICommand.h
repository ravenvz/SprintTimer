#ifndef ICOMMAND_H_NHSUJX60
#define ICOMMAND_H_NHSUJX60


class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
};


#endif /* end of include guard: ICOMMAND_H_NHSUJX60 */
