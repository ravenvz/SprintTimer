#ifndef FAKEPOMODORODISTRIBUTIONREADER_H_UREC2M3T
#define FAKEPOMODORODISTRIBUTIONREADER_H_UREC2M3T

#include "core/IPomodoroDistributionReader.h"
#include "FakeStorage.h"


class FakePomodoroDistributionReader : public IPomodoroDistributionReader {
public:
    FakePomodoroDistributionReader(FakeStorage<Pomodoro>& storage)
        : storage{storage}
    {
    }

    void requestDailyDistribution(const TimeSpan& timeSpan,
                                  Handler handler) final
    {
        return storage.requestDailyDistribution(timeSpan, handler);
    }

    FakeStorage<Pomodoro>& storage;
};

#endif /* end of include guard: FAKEPOMODORODISTRIBUTIONREADER_H_UREC2M3T */
