#ifndef SPRINTSTORAGEWRITERMOCK_H_DRJBQM7G
#define SPRINTSTORAGEWRITERMOCK_H_DRJBQM7G

#include <core/ISprintStorageWriter.h>
#include <gmock/gmock.h>

class SprintStorageWriterMock : public sprint_timer::ISprintStorageWriter {
public:
    MOCK_METHOD1(save, void(const sprint_timer::entities::Sprint&));
    MOCK_METHOD1(save,
                 void(const std::vector<sprint_timer::entities::Sprint>&));
    MOCK_METHOD1(remove, void(const sprint_timer::entities::Sprint&));
    MOCK_METHOD1(remove,
                 void(const std::vector<sprint_timer::entities::Sprint>&));
};

#endif /* end of include guard: SPRINTSTORAGEWRITERMOCK_H_DRJBQM7G */
