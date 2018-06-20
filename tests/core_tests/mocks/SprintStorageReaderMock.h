#ifndef SPRINTSTORAGEREADERMOCK_H_VCWBUOSQ
#define SPRINTSTORAGEREADERMOCK_H_VCWBUOSQ

#include <core/ISprintStorageReader.h>
#include <gmock/gmock.h>

class SprintStorageReaderMock : public ISprintStorageReader {
public:
    MOCK_METHOD2(requestItems,
                 void(const dw::TimeSpan&, ISprintStorageReader::Handler));
};

#endif /* end of include guard: SPRINTSTORAGEREADERMOCK_H_VCWBUOSQ */
