#ifndef REQUESTHANDLERCOMPOSER_H_GSJCXNMD
#define REQUESTHANDLERCOMPOSER_H_GSJCXNMD

#include "api_tests/CommandHandlerComposer.h"
#include "api_tests/QueryHandlerComposer.h"

namespace sprint_timer::compose {

struct RequestHandlerComposer : public CommandHandlerComposer,
                                public QueryHandlerComposer { };

} // namespace sprint_timer::compose

#endif /* end of include guard: REQUESTHANDLERCOMPOSER_H_GSJCXNMD */

