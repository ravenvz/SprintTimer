#ifndef FAKEPOMODOROYEARRANGEREADER_H_JWLLCTWA
#define FAKEPOMODOROYEARRANGEREADER_H_JWLLCTWA

#include "core/IPomodoroYearRangeReader.h"

class FakePomodoroYearRangeReader : public IPomodoroYearRangeReader {
public:
    void requestYearRange(Handler handler) final
    {
        return handler(std::vector<std::string>{"2015", "2016"});
    }

private:
    /* data */
};

#endif /* end of include guard: FAKEPOMODOROYEARRANGEREADER_H_JWLLCTWA */
