/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#ifndef BOOSTUUIDGENERATOR_H_OF6HXUYB
#define BOOSTUUIDGENERATOR_H_OF6HXUYB

#include "core/UUIDGenerator.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace sprint_timer {

class BoostUUIDGenerator : public UUIDGenerator {
public:
    std::string generateUUID() override { return to_string(generator()); }

private:
    boost::uuids::random_generator generator;
};

} // namespace sprint_timer

#endif /* end of include guard: BOOSTUUIDGENERATOR_H_OF6HXUYB */
