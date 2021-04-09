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
#ifndef WORKFLOWMOCK_H_NH6SQD7C
#define WORKFLOWMOCK_H_NH6SQD7C

#include <core/IWorkflow.h>
#include <gmock/gmock.h>

namespace mocks {

class WorkflowMock : public sprint_timer::IWorkflow {
public:
    MOCK_METHOD(void, start, (), (override));

    MOCK_METHOD(void, cancel, (), (override));

    MOCK_METHOD(std::chrono::seconds, currentDuration, (), (const override));

    MOCK_METHOD(void, toggleInTheZoneMode, (), (override));

    MOCK_METHOD(std::vector<dw::DateTimeRange>,
                completedSprints,
                (),
                (const override));

    MOCK_METHOD(void, setNumFinishedSprints, (int), (override));

    MOCK_METHOD(void, addListener, (IWorkflow::WorkflowListener*), (override));

    MOCK_METHOD(void,
                removeListener,
                (IWorkflow::WorkflowListener*),
                (override));

    MOCK_METHOD(void,
                reconfigure,
                (const IWorkflow::WorkflowParams&),
                (override));
};

} // namespace mocks

#endif /* end of include guard: WORKFLOWMOCK_H_NH6SQD7C */
