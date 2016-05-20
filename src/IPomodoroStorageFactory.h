#ifndef IPOMODOROSTORAGEFACTORY_H_PRIUAOH6
#define IPOMODOROSTORAGEFACTORY_H_PRIUAOH6

#include "core/IPomodoroDistributionReader.h"
#include "core/IPomodoroStorageReader.h"
#include "core/IPomodoroStorageWriter.h"
#include "core/IPomodoroYearRangeReader.h"
#include "db_layer/db_service.h"
#include <memory>

class IPomodoroStorageFactory {
public:
    virtual ~IPomodoroStorageFactory() = default;

    virtual std::unique_ptr<IPomodoroStorageReader>
    createPomodoroStorageReader() const = 0;

    virtual std::unique_ptr<IPomodoroStorageWriter>
    createPomodoroStorageWriter() const = 0;

    virtual std::unique_ptr<IPomodoroYearRangeReader>
    createPomodoroYearRangeReader() const = 0;

    virtual std::unique_ptr<IPomodoroDistributionReader>
    createPomoDailyDistributionReader() const = 0;

    virtual std::unique_ptr<IPomodoroDistributionReader>
    createPomoWeeklyDistributionReader() const = 0;

    virtual std::unique_ptr<IPomodoroDistributionReader>
    createPomoMonthlyDistributionReader() const = 0;
};

#endif /* end of include guard: IPOMODOROSTORAGEFACTORY_H_PRIUAOH6 */
