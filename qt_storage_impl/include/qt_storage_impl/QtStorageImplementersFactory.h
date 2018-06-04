/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef QT_STORAGE_IMPLEMENTERS_FACTORY_H_57Q0AHPC
#define QT_STORAGE_IMPLEMENTERS_FACTORY_H_57Q0AHPC

#include "core/IStorageImplementersFactory.h"
#include "qt_storage_impl/QtSprintDistributionReader.h"
#include "qt_storage_impl/QtSprintStorageReader.h"
#include "qt_storage_impl/QtSprintStorageWriter.h"
#include "qt_storage_impl/QtTaskStorageReader.h"
#include "qt_storage_impl/QtTaskStorageWriter.h"
#include "qt_storage_impl/QtYearRangeReader.h"

class QtStorageImplementersFactory : public IStorageImplementersFactory {
public:
    explicit QtStorageImplementersFactory(DBService& dbService)
            : dbService{dbService}
    {
    }

    std::unique_ptr<ISprintStorageReader>
    createSprintStorageReader() const override
    {
        return std::make_unique<QtSprintStorageReader>(dbService);
    }

    std::unique_ptr<ISprintStorageWriter>
    createSprintStorageWriter() const override
    {
        return std::make_unique<QtSprintStorageWriter>(dbService);
    }

    std::unique_ptr<IYearRangeReader>
    createYearRangeReader() const override
    {
        return std::make_unique<QtYearRangeReader>(dbService);
    }

    std::unique_ptr<ISprintDistributionReader>
    createSprintDailyDistributionReader() const override
    {
        constexpr size_t numDays{30};
        return std::make_unique<QtSprintDailyDistributionReader>(dbService, numDays);
    }

    std::unique_ptr<ISprintDistributionReader>
    createSprintWeeklyDistributionReader() const override
    {
        constexpr size_t numWeeks{12};
        return std::make_unique<QtSprintWeeklyDistributionReader>(dbService, numWeeks);
    }

    std::unique_ptr<ISprintDistributionReader>
    createSprintMonthlyDistributionReader() const override
    {
        constexpr size_t numMonths{12};
        return std::make_unique<QtSprintMonthlyDistributionReader>(dbService, numMonths);
    }

    std::unique_ptr<ITaskStorageReader> createTaskStorageReader() const override
    {
        return std::make_unique<QtTaskStorageReader>(dbService);
    }

    std::unique_ptr<ITaskStorageWriter> createTaskStorageWriter() const override
    {
        return std::make_unique<QtTaskStorageWriter>(dbService);
    }

private:
    DBService& dbService;
};

#endif /* end of include guard: QT_STORAGE_IMPLEMENTERS_FACTORY_H_57Q0AHPC */
