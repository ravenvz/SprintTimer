#ifndef YEARRANGEREADERMOCK_H_RFGJ7KUC
#define YEARRANGEREADERMOCK_H_RFGJ7KUC

#include <core/IYearRangeReader.h>
#include <gmock/gmock.h>

class YearRangeReaderMock : public IYearRangeReader {
public:
    MOCK_METHOD1(requestYearRange, void(IYearRangeReader::Handler));
};

#endif /* end of include guard: YEARRANGEREADERMOCK_H_RFGJ7KUC */
