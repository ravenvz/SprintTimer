#include "core/use_cases/use_cases.h"
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <TestHarness.h>


using namespace UseCases;
using std::experimental::optional;
using std::experimental::make_optional;


// struct PomoStorageItem {
//     long long id;
//     long long associatedTaskId;
//     TimeSpan timeSpan;
// };
//
// bool storageItemEqual(const PomoStorageItem& lhs, const PomoStorageItem& rhs)
// {
//     return lhs.id == rhs.id && lhs.associatedTaskId == rhs.associatedTaskId;
// }
//
// class FakeStorage {
// public:
//     long long store(long long associatedTaskId, const TimeSpan& timeSpan)
//     {
//         long long id = nextId;
//         storage.insert(std::make_pair(id, PomoStorageItem{id,
//         associatedTaskId, timeSpan}));
//         ++nextId;
//         return id;
//     }
//
//     bool contains(long long id)
//     {
//         auto found = storage.find(id);
//         return found != cend(storage);
//     }
//
//     optional<IPomodoroStorageWriter::PomoData> remove(long long id)
//     {
//         auto found = storage.find(id);
//         if (found == cend(storage)) {
//             return optional<IPomodoroStorageWriter::PomoData>();
//         }
//         PomoStorageItem item = found->second;
//         optional<IPomodoroStorageWriter::PomoData> result
//             = make_optional(std::make_pair(item.associatedTaskId,
//             item.timeSpan));
//         storage.erase(found);
//         return result;
//     }
//
//     size_t size() const {
//         return storage.size();
//     }
//
// private:
//     long long nextId{0};
//     std::unordered_map<long long, PomoStorageItem> storage;
// };
//
// #<{(| Fake implementation of IPomodoroStorageWriter that exposes
//  * internal storage and allows to test execute/undo. |)}>#
// class TestPomodoroGateway : public IPomodoroStorageWriter {
// public:
//     optional<long long> save(long long associatedTaskId,
//                              const TimeSpan& timeSpan) final
//     {
//         return make_optional(storage.store(associatedTaskId, timeSpan));
//     }
//
//     optional<PomoData> remove(long long pomodoroId) final {
//         return storage.remove(pomodoroId);
//     }
//
//     FakeStorage storage;
// };
//
// class AlwaysSuccessfulWriter : public IPomodoroStorageWriter {
// public:
//     optional<long long> save(long long associatedTaskId,
//                              const TimeSpan& timeSpan) final
//     {
//         return make_optional(5ll);
//     }
//     optional<PomoData> remove(long long pomodoroId) final
//     {
//         return make_optional(
//             std::make_pair(73ll,
//                            TimeSpan{DateTime::currentDateTime(),
//                                     DateTime::currentDateTime()}));
//     }
// };
//
// class AlwaysFailingWriter : public IPomodoroStorageWriter {
// public:
//     optional<long long> save(long long associatedTaskId,
//                              const TimeSpan& timeSpan) final
//     {
//         return optional<long long>();
//     }
//     optional<PomoData> remove(long long pomodoroId) final
//     {
//         return optional<PomoData>();
//     }
// };
//
// TEST_GROUP(TestAddPomodoroTransaction)
// {
//     const TimeSpan defaultTimeSpan
//         = TimeSpan{DateTime::currentDateTime(), DateTime::currentDateTime()};
//     const long long defaultTaskId{77};
// };
//
// TEST(TestAddPomodoroTransaction, test_should_return_true_on_success)
// {
//     AlwaysSuccessfulWriter gateway;
//     AddPomodoroTransaction add_pomodoro{
//         gateway, defaultTaskId, defaultTimeSpan};
//
//     CHECK(add_pomodoro.execute());
// }
//
// TEST(TestAddPomodoroTransaction, test_should_return_false_when_adding_failed)
// {
//     AlwaysFailingWriter gateway;
//     AddPomodoroTransaction add_pomodoro{
//         gateway, defaultTaskId, defaultTimeSpan};
//
//     CHECK(!add_pomodoro.execute());
// }
//
// TEST(TestAddPomodoroTransaction,
//      test_undo_transaction_that_has_not_been_executed_should_return_false)
// {
//     TestPomodoroGateway gateway;
//     AddPomodoroTransaction add_pomodoro{
//         gateway, defaultTaskId, defaultTimeSpan};
//
//     CHECK(!add_pomodoro.undo());
// }
//
// TEST(TestAddPomodoroTransaction, test_execute_and_undo)
// {
//     TestPomodoroGateway gateway;
//     AddPomodoroTransaction add_pomodoro{
//         gateway, defaultTaskId, defaultTimeSpan};
//
//     CHECK(add_pomodoro.execute());
//     CHECK_EQUAL(1, gateway.storage.size());
//     CHECK(add_pomodoro.undo());
//     CHECK_EQUAL(0, gateway.storage.size());
// }
//
// TEST_GROUP(RemovePomodoroTransaction){
//     const TimeSpan defaultTimeSpan
//         = TimeSpan{DateTime::currentDateTime(), DateTime::currentDateTime()};
//     const long long defaultTaskId{77};
// };
//
// TEST(RemovePomodoroTransaction,
//      test_undo_should_not_add_pomodoro_if_it_was_not_removed_beforehand)
// {
//     TestPomodoroGateway gateway;
//     auto id = gateway.storage.store(defaultTaskId, defaultTimeSpan);
//     RemovePomodoroTransaction removePomodoro{gateway, id};
//
//     CHECK(!removePomodoro.undo());
//     CHECK_EQUAL(1, gateway.storage.size())
// }
//
// TEST(RemovePomodoroTransaction, test_remove_and_undo)
// {
//     TestPomodoroGateway gateway;
//     auto id = gateway.storage.store(defaultTaskId, defaultTimeSpan);
//     RemovePomodoroTransaction removePomodoro{gateway, id};
//
//     CHECK(removePomodoro.execute());
//     CHECK(gateway.storage.size() == 0);
//     CHECK(removePomodoro.undo());
//     CHECK_EQUAL(1, gateway.storage.size());
// }
