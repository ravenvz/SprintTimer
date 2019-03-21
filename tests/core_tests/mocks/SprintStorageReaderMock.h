#ifndef SPRINTSTORAGEREADERMOCK_H_VCWBUOSQ
#define SPRINTSTORAGEREADERMOCK_H_VCWBUOSQ

#include <core/ISprintStorageReader.h>
#include <gmock/gmock.h>

class SprintStorageReaderMock : public sprint_timer::ISprintStorageReader {
public:
    MOCK_METHOD2(requestItems,
                 void(const dw::DateTimeRange&,
                      sprint_timer::ISprintStorageReader::Handler));
    MOCK_METHOD2(sprintsForTask,
                 void(const std::string& taskUuid,
                      sprint_timer::ISprintStorageReader::Handler));
};

#endif /* end of include guard: SPRINTSTORAGEREADERMOCK_H_VCWBUOSQ */
