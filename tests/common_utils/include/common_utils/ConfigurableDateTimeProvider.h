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
#ifndef CONFIGURABLEDATETIMEPROVIDER_H_DKMDAIXR
#define CONFIGURABLEDATETIMEPROVIDER_H_DKMDAIXR

#include "api/DateTimeProvider.h"
#include <memory>

class ConfigurableDateTimeProvider
    : public sprint_timer::api::DateTimeProvider {
public:
    explicit ConfigurableDateTimeProvider(
        std::unique_ptr<sprint_timer::api::DateTimeProvider> wrapped_)
        : wrapped{std::move(wrapped_)}
    {
    }

    ConfigurableDateTimeProvider(
        std::unique_ptr<sprint_timer::api::DateTimeProvider> wrapped_,
        dw::DateTime fixedTimepoint)
        : wrapped{std::move(wrapped_)}
        , anchorTime{fixedTimepoint}
    {
    }

    auto setAnchorTime(std::optional<dw::DateTime> fixedTimepoint) -> void
    {
        anchorTime = std::move(fixedTimepoint);
    }

    auto currentTimePoint() const -> std::optional<dw::DateTime>
    {
        return anchorTime;
    }

    auto resetAnchorTime() -> void { anchorTime = std::nullopt; }

    auto dateNowImpl() const -> dw::Date override
    {
        return anchorTime.value_or(wrapped->dateTimeNow()).date();
    }

    auto dateTimeNowImpl() const -> dw::DateTime override
    {
        return anchorTime.value_or(wrapped->dateTimeNow());
    }

    auto dateLocalNowImpl() const -> dw::Date override
    {
        return anchorTime.value_or(wrapped->dateTimeLocalNow()).date();
    }

    auto dateTimeLocalNowImpl() const -> dw::DateTime override
    {
        return anchorTime.value_or(wrapped->dateTimeLocalNow());
    }

private:
    std::unique_ptr<sprint_timer::api::DateTimeProvider> wrapped;
    std::optional<dw::DateTime> anchorTime{};
};

/* Congfigures date time provider to use specified time point and restores when
 * destroyed. */
class TimePortalGuard {
public:
    TimePortalGuard(ConfigurableDateTimeProvider& dateTimeProvider_,
                    dw::DateTime fixedTimepoint_)
        : dateTimeProvider{dateTimeProvider_}
        , previousTimePoint{dateTimeProvider_.currentTimePoint()}
    {
        dateTimeProvider.setAnchorTime(fixedTimepoint_);
    }

    ~TimePortalGuard() { dateTimeProvider.setAnchorTime(previousTimePoint); }

    TimePortalGuard(const TimePortalGuard&) = delete;
    TimePortalGuard(TimePortalGuard&&) = delete;
    TimePortalGuard& operator=(const TimePortalGuard&) = delete;
    TimePortalGuard& operator=(TimePortalGuard&&) = delete;

private:
    ConfigurableDateTimeProvider& dateTimeProvider;
    std::optional<dw::DateTime> previousTimePoint;
};

#endif /* end of include guard: CONFIGURABLEDATETIMEPROVIDER_H_DKMDAIXR */
