#ifndef SPRINTDISTRIBUTIONREADERMOCK_H_RNDBJKUR
#define SPRINTDISTRIBUTIONREADERMOCK_H_RNDBJKUR

#include <core/ISprintDistributionReader.h>
#include <gmock/gmock.h>

class SprintDistributionReaderMock : public ISprintDistributionReader {
public:
    MOCK_METHOD2(requestDistribution,
                 void(const dw::TimeSpan&, ISprintDistributionReader::Handler));
};

#endif /* end of include guard: SPRINTDISTRIBUTIONREADERMOCK_H_RNDBJKUR */
