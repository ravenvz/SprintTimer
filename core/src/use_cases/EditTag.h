#ifndef EDITTAG_H_6NGOS1Q2
#define EDITTAG_H_6NGOS1Q2

#include "core/ITaskStorageWriter.h"
#include "core/RevertableCommand.h"
#include <iostream>

namespace UseCases {

class EditTag : public RevertableCommand {
public:
    EditTag(ITaskStorageWriter& tagStorageWriter,
            const std::string& oldName,
            const std::string& newName)
        : writer{tagStorageWriter}
        , oldName{oldName}
        , newName{newName}
    {
    }

protected:
    void executeAction() final { writer.editTag(oldName, newName); }

    void undoAction() final { writer.editTag(newName, oldName); }

private:
    ITaskStorageWriter& writer;
    const std::string oldName;
    const std::string newName;
};

} /* UseCases */

#endif /* end of include guard: EDITTAG_H_6NGOS1Q2 */
