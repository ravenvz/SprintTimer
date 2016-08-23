/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef QT_STORAGE_IMPLEMENTERS_FACTORY_H_57Q0AHPC
#define QT_STORAGE_IMPLEMENTERS_FACTORY_H_57Q0AHPC

#include "core/IStorageImplementersFactory.h"
#include "qt_storage_impl/QtSprintDistributionReader.h"
#include "qt_storage_impl/QtSprintStorageReader.h"
#include "qt_storage_impl/QtSprintStorageWriter.h"
#include "qt_storage_impl/QtYearRangeReader.h"
#include "qt_storage_impl/QtTaskStorageReader.h"
#include "qt_storage_impl/QtTaskStorageWriter.h"

class QtStorageImplementersFactory : public IStorageImplementersFactory {
public:
    QtStorageImplementersFactory(DBService& dbService)
        : dbService{dbService}
    {
    }

    std::unique_ptr<ISprintStorageReader>
    createSprintStorageReader() const final
    {
        return std::make_unique<QtSprintStorageReader>(dbService);
    }

    std::unique_ptr<ISprintStorageWriter>
    createSprintStorageWriter() const final
    {
        return std::make_unique<QtSprintStorageWriter>(dbService);
    }

    std::unique_ptr<IYearRangeReader>
    createYearRangeReader() const final
    {
        return std::make_unique<QtYearRangeReader>(dbService);
    }

    std::unique_ptr<ISprintDistributionReader>
    createSprintDailyDistributionReader() const final
    {
        return std::make_unique<QtSprintDailyDistributionReader>(dbService);
    }

    std::unique_ptr<ISprintDistributionReader>
    createSprintWeeklyDistributionReader() const final
    {
        return std::make_unique<QtSprintWeeklyDistributionReader>(dbService);
    }

    std::unique_ptr<ISprintDistributionReader>
    createSprintMonthlyDistributionReader() const final
    {
        return std::make_unique<QtSprintMonthlyDistributionReader>(dbService);
    }

    std::unique_ptr<ITaskStorageReader> createTaskStorageReader() const final
    {
        return std::make_unique<QtTaskStorageReader>(dbService);
    }

    std::unique_ptr<ITaskStorageWriter> createTaskStorageWriter() const final
    {
        return std::make_unique<QtTaskStorageWriter>(dbService);
    }

private:
    DBService& dbService;
};

#endif /* end of include guard: QT_STORAGE_IMPLEMENTERS_FACTORY_H_57Q0AHPC */
