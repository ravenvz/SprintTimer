#include "qt_storage_impl/QtStorageImplementersFactory.h"

namespace sprint_timer::storage::qt_storage_impl {

QtStorageImplementersFactory::QtStorageImplementersFactory(DBService& dbService)
    : dbService{dbService}
{
}

std::unique_ptr<ISprintStorageReader>
QtStorageImplementersFactory::createSprintStorageReader() const
{
    return std::make_unique<QtSprintStorageReader>(dbService);
}

std::unique_ptr<ISprintStorageWriter>
QtStorageImplementersFactory::createSprintStorageWriter() const
{
    return std::make_unique<QtSprintStorageWriter>(dbService);
}

std::unique_ptr<IYearRangeReader>
QtStorageImplementersFactory::createYearRangeReader() const
{
    return std::make_unique<QtYearRangeReader>(dbService);
}

std::unique_ptr<ISprintDistributionReader>
QtStorageImplementersFactory::createSprintDailyDistributionReader() const
{
    constexpr size_t numDays{30};
    return std::make_unique<QtSprintDailyDistributionReader>(dbService,
                                                             numDays);
}

std::unique_ptr<ISprintDistributionReader>
QtStorageImplementersFactory::createSprintWeeklyDistributionReader(
    FirstDayOfWeek firstDayOfWeek) const
{
    constexpr size_t numWeeks{12};
    if (firstDayOfWeek == FirstDayOfWeek::Monday)
        return std::make_unique<QtSprintDistReaderMondayFirst>(dbService,
                                                               numWeeks);
    return std::make_unique<QtSprintDistReaderSundayFirst>(dbService, numWeeks);
}

std::unique_ptr<ISprintDistributionReader>
QtStorageImplementersFactory::createSprintMonthlyDistributionReader() const
{
    constexpr size_t numMonths{12};
    return std::make_unique<QtSprintMonthlyDistributionReader>(dbService,
                                                               numMonths);
}

std::unique_ptr<ITaskStorageReader>
QtStorageImplementersFactory::createTaskStorageReader() const
{
    return std::make_unique<QtTaskStorageReader>(dbService);
}

std::unique_ptr<ITaskStorageWriter>
QtStorageImplementersFactory::createTaskStorageWriter() const
{
    return std::make_unique<QtTaskStorageWriter>(dbService);
}

} // namespace sprint_timer::storage::qt_storage_impl
