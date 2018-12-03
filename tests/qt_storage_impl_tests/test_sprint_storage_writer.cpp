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
#include "gtest/gtest.h"
#include <qt_storage_impl/DBService.h>
#include <qt_storage_impl/QtStorageImplementersFactory.h>

using namespace sprint_timer::storage::qt_storage_impl;

class SprintStorageWriterFixture : public ::testing::Test {
public:
    // DBService dbService{QString{":memory:"}};
    // QtStorageImplementersFactory factory{dbService};
    // std::unique_ptr<sprint_timer::ISprintStorageWriter> sprintStorageWriter
    //     = factory.createSprintStorageWriter();
};

TEST_F(SprintStorageWriterFixture, save_sprint) {}
