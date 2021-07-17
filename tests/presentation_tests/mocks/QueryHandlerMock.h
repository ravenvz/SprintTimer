/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#ifndef QUERYHANDLERMOCK_H_BJVH2ZLU
#define QUERYHANDLERMOCK_H_BJVH2ZLU

#include "core/AsyncQueryHandler.h"
#include "core/QueryHandler.h"
#include "gmock/gmock.h"

namespace mocks {

template <typename QueryT>
class QueryHandlerMock : public sprint_timer::QueryHandler<QueryT> {
public:
    MOCK_METHOD(typename QueryT::result_t, handle, (QueryT &&), (override));
};

template <typename QueryT>
class AsyncQueryHandlerMock : public sprint_timer::AsyncQueryHandler<QueryT> {
public:
    MOCK_METHOD(std::future<typename QueryT::result_t>,
                handleAwaitImpl,
                (QueryT &&),
                (override));

    MOCK_METHOD(typename QueryT::result_t,
                handleSyncImpl,
                (QueryT &&),
                (override));

    MOCK_METHOD(void,
                handleCallbackImpl,
                (QueryT&&, std::function<void(typename QueryT::result_t)>),
                (override));
};

template <typename QueryT>
void given_handler_returns(
    testing::NiceMock<mocks::QueryHandlerMock<QueryT>>& handler,
    typename QueryT::result_t result)
{
    ON_CALL(handler, handle(testing::_)).WillByDefault(testing::Return(result));
}

template <typename QueryT>
void given_handler_returns(
    testing::NiceMock<mocks::AsyncQueryHandlerMock<QueryT>>& handler,
    typename QueryT::result_t result)
{
    std::promise<typename QueryT::result_t> p;
    p.set_value(result);
    ON_CALL(handler, handleAwaitImpl(testing::_))
        .WillByDefault(testing::Return(testing::ByMove(p.get_future())));
}

} // namespace mocks

#endif /* end of include guard: QUERYHANDLERMOCK_H_BJVH2ZLU */
