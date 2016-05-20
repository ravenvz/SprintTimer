#ifndef QTPOMODOROSTORAGEFACTORY_H_57Q0AHPC
#define QTPOMODOROSTORAGEFACTORY_H_57Q0AHPC

#include "IPomodoroStorageFactory.h"
#include "qt_storage_impl/QtPomoDistributionReader.h"
#include "qt_storage_impl/QtPomoStorageReader.h"
#include "qt_storage_impl/QtPomoStorageWriter.h"
#include "qt_storage_impl/QtPomoYearRangeReader.h"

class QtPomodoroStorageFactory : public IPomodoroStorageFactory {
public:
    QtPomodoroStorageFactory(DBService& dbService)
        : dbService{dbService}
    {
    }

    std::unique_ptr<IPomodoroStorageReader>
    createPomodoroStorageReader() const final
    {
        return std::make_unique<QtPomoStorageReader>(dbService);
    }

    std::unique_ptr<IPomodoroStorageWriter>
    createPomodoroStorageWriter() const final
    {
        return std::make_unique<QtPomoStorageWriter>(dbService);
    }

    std::unique_ptr<IPomodoroYearRangeReader>
    createPomodoroYearRangeReader() const final
    {
        return std::make_unique<QtPomoYearRangeReader>(dbService);
    }

    std::unique_ptr<IPomodoroDistributionReader>
    createPomoDailyDistributionReader() const final
    {
        return std::make_unique<QtPomoDailyDistributionReader>(dbService);
    }

    std::unique_ptr<IPomodoroDistributionReader>
    createPomoWeeklyDistributionReader() const final
    {
        return std::make_unique<QtPomoWeeklyDistributionReader>(dbService);
    }

    std::unique_ptr<IPomodoroDistributionReader>
    createPomoMonthlyDistributionReader() const final
    {
        return std::make_unique<QtPomoMonthlyDistributionReader>(dbService);
    }

private:
    DBService& dbService;
};

#endif /* end of include guard: QTPOMODOROSTORAGEFACTORY_H_57Q0AHPC */
