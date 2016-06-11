#ifndef REQUESTALLTAGS_H_CUXYESDS
#define REQUESTALLTAGS_H_CUXYESDS

#include "core/Command.h"
#include "core/ITaskStorageReader.h"

namespace UseCases {

class RequestAllTags : public Command {
public:
    RequestAllTags(ITaskStorageReader& taskStorageReader,
                   std::function<void(const std::vector<std::string>&)> handler)
        : reader{taskStorageReader}
        , handler{handler}
    {
    }

    void execute() final { reader.requestAllTags(handler); }

    std::string inspect() const final { return "Request all tags"; }

private:
    ITaskStorageReader& reader;
    std::function<void(const std::vector<std::string>&)> handler;
};

} /* UseCases */

#endif /* end of include guard: REQUESTALLTAGS_H_CUXYESDS */
