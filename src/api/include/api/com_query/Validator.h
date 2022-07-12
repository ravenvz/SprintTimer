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
#ifndef VALIDATOR_H_FXQUVDDC
#define VALIDATOR_H_FXQUVDDC

#include <optional>
#include <string>

namespace asp {

template <typename RequestT> class Validator {
public:
    virtual ~Validator() = default;

    virtual std::optional<std::string> validate(const RequestT& request) const = 0;
};

} // namespace asp

#endif /* end of include guard: VALIDATOR_H_FXQUVDDC */
