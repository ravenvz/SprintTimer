#ifndef FAKEPOMODOROSTORAGEREADER_H_MFYC8R5A
#define FAKEPOMODOROSTORAGEREADER_H_MFYC8R5A

#include "core/IPomodoroStorageReader.h"
#include "FakeStorage.h"

class FakePomodoroStorageReader : public IPomodoroStorageReader {
public:
    FakePomodoroStorageReader(FakeStorage<Pomodoro>& storage)
        : storage{storage}
    {
    }

    void requestItems(const TimeSpan& timeSpan, Handler handler) final
    {
        storage.itemsInTimeRange(timeSpan, handler);
    }

private:
    FakeStorage<Pomodoro>& storage;
};

#endif /* end of include guard: FAKEPOMODOROSTORAGEREADER_H_MFYC8R5A */
