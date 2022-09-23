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
#ifndef DOMAINEXCEPTION_H
#define DOMAINEXCEPTION_H

#include "core/SprintTimerException.h"
#include <string>

namespace sprint_timer {

class DomainException : public SprintTimerException {
public:
    explicit DomainException(const std::string& message)
        : SprintTimerException{message}
    {
    }
};

} // namespace sprint_timer

#endif /* end of include guard: DOMAINEXCEPTION_H */
