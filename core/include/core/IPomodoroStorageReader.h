#ifndef IPOMODOROSTORAGEREADER_H_UJ8CZPXS
#define IPOMODOROSTORAGEREADER_H_UJ8CZPXS

#include "core/entities/Pomodoro.h"
#include <functional>


class IPomodoroStorageReader {
public:
    using Items = std::vector<Pomodoro>;
    using Handler = std::function<void(const Items&)>;
    virtual ~IPomodoroStorageReader() = default;
    virtual void requestItems(const TimeSpan& timeSpan, Handler handler) = 0;
};


#endif /* end of include guard: IPOMODOROSTORAGEREADER_H_UJ8CZPXS */
