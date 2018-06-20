#ifndef TASKSTORAGEREADERMOCK_H_P93DAESG
#define TASKSTORAGEREADERMOCK_H_P93DAESG

#include <core/ITaskStorageReader.h>
#include <gmock/gmock.h>

class TaskStorageReaderMock : public ITaskStorageReader {
public:
    MOCK_METHOD1(requestUnfinishedTasks, void(ITaskStorageReader::Handler));
    MOCK_METHOD2(requestFinishedTasks,
                 void(const dw::TimeSpan&, ITaskStorageReader::Handler));
    MOCK_METHOD2(requestTasks,
                 void(const dw::TimeSpan&, ITaskStorageReader::Handler));
    MOCK_METHOD1(requestAllTags, void(ITaskStorageReader::TagHandler));
};

#endif /* end of include guard: TASKSTORAGEREADERMOCK_H_P93DAESG */
