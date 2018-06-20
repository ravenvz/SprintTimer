#ifndef TASKSTORAGEWRITERMOCK_H_K4RA0LHC
#define TASKSTORAGEWRITERMOCK_H_K4RA0LHC

#include <core/ITaskStorageWriter.h>
#include <gmock/gmock.h>

class TaskStorageWriterMock : public ITaskStorageWriter {
public:
    MOCK_METHOD1(save, void(const Task&));
    MOCK_METHOD1(remove, void(const Task&));
    MOCK_METHOD2(edit, void(const Task&, const Task&));
    MOCK_METHOD1(incrementSprints, void(const std::string&));
    MOCK_METHOD1(decrementSprints, void(const std::string&));
    MOCK_METHOD2(toggleTaskCompletionStatus,
                 void(const std::string&, const dw::DateTime&));
    MOCK_METHOD1(updatePriorities, void(const std::vector<std::string>&));
    MOCK_METHOD2(editTag, void(const std::string&, const std::string&));
};

#endif /* end of include guard: TASKSTORAGEWRITERMOCK_H_K4RA0LHC */
