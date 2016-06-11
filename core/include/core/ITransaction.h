#ifndef ITRANSACTION_H_USZOOY0D
#define ITRANSACTION_H_USZOOY0D

class ITransaction {
public:
    virtual ~ITransaction() = default;
    virtual bool execute() = 0;
    virtual bool undo() = 0;
};


#endif /* end of include guard: ITRANSACTION_H_USZOOY0D */
