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
#ifndef IPOMODOROSTORAGEFACTORY_H_PRIUAOH6
#define IPOMODOROSTORAGEFACTORY_H_PRIUAOH6

#include "core/IPomodoroDistributionReader.h"
#include "core/IPomodoroStorageReader.h"
#include "core/IPomodoroStorageWriter.h"
#include "core/IPomodoroYearRangeReader.h"
#include "core/ITaskStorageReader.h"
#include "core/ITaskStorageWriter.h"
#include <memory>

class IStorageImplementersFactory {
public:
    virtual ~IStorageImplementersFactory() = default;

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

    virtual std::unique_ptr<ITaskStorageReader>
    createTaskStorageReader() const = 0;

    virtual std::unique_ptr<ITaskStorageWriter>
    createTaskStorageWriter() const = 0;
};

#endif /* end of include guard: IPOMODOROSTORAGEFACTORY_H_PRIUAOH6 */
