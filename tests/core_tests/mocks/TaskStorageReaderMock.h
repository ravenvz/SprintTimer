#ifndef TASKSTORAGEREADERMOCK_H_P93DAESG
#define TASKSTORAGEREADERMOCK_H_P93DAESG

#include <core/ITaskStorageReader.h>
#include <gmock/gmock.h>

class TaskStorageReaderMock : public sprint_timer::ITaskStorageReader {
public:
    MOCK_METHOD1(requestUnfinishedTasks,
                 void(sprint_timer::ITaskStorageReader::Handler));
    MOCK_METHOD2(requestFinishedTasks,
                 void(const dw::TimeSpan&,
                      sprint_timer::ITaskStorageReader::Handler));
    MOCK_METHOD2(requestTasks,
                 void(const dw::TimeSpan&,
                      sprint_timer::ITaskStorageReader::Handler));
    MOCK_METHOD1(requestAllTags,
                 void(sprint_timer::ITaskStorageReader::TagHandler));
};

#endif /* end of include guard: TASKSTORAGEREADERMOCK_H_P93DAESG */
