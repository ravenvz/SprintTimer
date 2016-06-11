#ifndef IPOMODOROYEARRANGEREADER_H_EIN38BSX
#define IPOMODOROYEARRANGEREADER_H_EIN38BSX

#include <functional>
#include <list>
#include <vector>

class IPomodoroYearRangeReader {
public:
    using Handler = std::function<void(const std::vector<std::string>&)>;
    virtual ~IPomodoroYearRangeReader() = default;
    virtual void requestYearRange(Handler handler) = 0;
};


#endif /* end of include guard: IPOMODOROYEARRANGEREADER_H_EIN38BSX */
