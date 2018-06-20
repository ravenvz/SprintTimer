#ifndef SPRINTSTORAGEWRITERMOCK_H_DRJBQM7G
#define SPRINTSTORAGEWRITERMOCK_H_DRJBQM7G

#include <core/ISprintStorageWriter.h>
#include <gmock/gmock.h>

class SprintStorageWriterMock : public ISprintStorageWriter {
public:
    MOCK_METHOD1(save, void(const Sprint&));
    MOCK_METHOD1(remove, void(const Sprint&));
};

#endif /* end of include guard: SPRINTSTORAGEWRITERMOCK_H_DRJBQM7G */
