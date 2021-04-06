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
#ifndef CONFIGMOCK_H_OVJZ2UQN
#define CONFIGMOCK_H_OVJZ2UQN

#include <core/IConfig.h>
#include <gmock/gmock.h>

class ConfigMock : public sprint_timer::IConfig {
public:
    MOCK_CONST_METHOD0(sprintDuration, std::chrono::minutes());
    MOCK_METHOD1(setSprintDuration, void(std::chrono::minutes));
    MOCK_CONST_METHOD0(shortBreakDuration, std::chrono::minutes());
    MOCK_METHOD1(setShortBreakDuration, void(std::chrono::minutes));
    MOCK_CONST_METHOD0(longBreakDuration, std::chrono::minutes());
    MOCK_METHOD1(setLongBreakDuration, void(std::chrono::minutes));
    MOCK_CONST_METHOD0(numSprintsBeforeBreak, int());
    MOCK_METHOD1(setNumSprintsBeforeBreak, void(int));
    MOCK_CONST_METHOD0(soundIsEnabled, bool());
    MOCK_METHOD1(setPlaySound, void(bool));
    MOCK_CONST_METHOD0(soundVolume, int());
    MOCK_METHOD1(setSoundVolume, void(int));
    MOCK_CONST_METHOD0(soundFilePath, std::string());
    MOCK_METHOD1(setSoundFilePath, void(const std::string&));
    MOCK_CONST_METHOD0(timerFlavour, int());
    MOCK_METHOD1(setTimerFlavour, void(int));
    MOCK_CONST_METHOD0(firstDayOfWeek, dw::Weekday());
    MOCK_METHOD1(setFirstDayOfWeek, void(dw::Weekday));
};


#endif /* end of include guard: CONFIGMOCK_H_OVJZ2UQN */
