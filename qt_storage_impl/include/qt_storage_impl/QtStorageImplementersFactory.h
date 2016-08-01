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
#ifndef QTPOMODOROSTORAGEFACTORY_H_57Q0AHPC
#define QTPOMODOROSTORAGEFACTORY_H_57Q0AHPC

#include "core/IStorageImplementersFactory.h"
#include "qt_storage_impl/QtPomoDistributionReader.h"
#include "qt_storage_impl/QtPomoStorageReader.h"
#include "qt_storage_impl/QtPomoStorageWriter.h"
#include "qt_storage_impl/QtPomoYearRangeReader.h"
#include "qt_storage_impl/QtTaskStorageReader.h"
#include "qt_storage_impl/QtTaskStorageWriter.h"

class QtStorageImplementersFactory : public IStorageImplementersFactory {
public:
    QtStorageImplementersFactory(DBService& dbService)
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

#endif /* end of include guard: QTPOMODOROSTORAGEFACTORY_H_57Q0AHPC */
