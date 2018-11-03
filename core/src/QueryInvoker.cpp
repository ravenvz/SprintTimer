#include "core/QueryInvoker.h"

namespace sprint_timer {

void QueryInvoker::execute(std::unique_ptr<Query> query) const
{
    query->execute();
}

} // namespace sprint_timer
