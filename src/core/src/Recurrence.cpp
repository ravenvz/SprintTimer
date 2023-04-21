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
#include "core/Recurrence.h"
#include "core/utils/Algutils.h"
#include "core/utils/StringUtils.h"
#include <array>
#include <format>
#include <functional>
#include <iomanip>
#include <iostream>
#include <optional>
#include <ranges>
#include <variant>

using namespace dw;
using sprint_timer::utils::split;
using namespace std::literals::string_view_literals;

namespace {

template <typename T> using Maybe = std::optional<T>;

using Pattern = std::string_view;

using SelectedMonths = std::bitset<13>;
using SelectedDays = std::bitset<32>;
using SelectedWeekdays = std::bitset<7>;
using SelectedYears = std::vector<int>;
using SelectedHours = std::bitset<24>;
using SelectedMinutes = std::bitset<60>;
using SelectedSeconds = std::bitset<60>;

struct IntRange {
    uint16_t min;
    uint16_t max;

    constexpr auto operator()(uint16_t value) const noexcept -> bool
    {
        return min <= value and value <= max;
    }

    auto valuesView() const -> auto { return std::views::iota(min, max + 1u); }
};

constexpr IntRange validYearRange{1970, 2500};
constexpr IntRange validMonthRange{1, 12};
constexpr IntRange validDayRange{1, 31};
constexpr IntRange validHourRange{0, 23};
constexpr IntRange validMinuteRange{0, 59};
constexpr IntRange validSecondRange{0, 59};

constexpr std::array<std::string_view, 7> dayNames{
    "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

template <typename Key, typename Value, std::size_t Size> struct Dictionary {
    std::array<std::pair<Key, Value>, Size> data;

    [[nodiscard]] constexpr auto at(const Key& key) const -> Value
    {
        const auto it =
            std::find_if(cbegin(data), cend(data), [&key](const auto& entry) {
                return entry.first == key;
            });
        if (it == cend(data)) {
            throw std::range_error{"Key not found in Dictionary"};
        }
        return it->second;
    }

    [[nodiscard]] constexpr auto contains(const Key& key) const noexcept -> bool
    {
        return std::find_if(
                   cbegin(data), cend(data), [&key](const auto& entry) {
                       return entry.first == key;
                   }) != cend(data);
    }
};

constexpr std::array<std::pair<std::string_view, std::string_view>, 8>
    shorthands{{{"hourly"sv, "*-*-* *:00:00"sv},
                {"daily"sv, "*-*-* 00:00:00"sv},
                {"monthly"sv, "*-*-01 00:00:00"sv},
                {"weekly"sv, "Mon *-*-* 00:00:00"sv},
                {"quarterly"sv, "*-01,04,07,10-01 00:00:00"sv},
                {"semiannually"sv, "*-01,07-01 00:00:00"sv},
                {"yearly"sv, "*-01-01 00:00:00"sv},
                {"annually"sv, "*-01-01 00:00:00"sv}}};

constexpr size_t weekdayPos{0};
constexpr size_t datePos{1};
constexpr size_t timePos{2};

struct Component {
    uint16_t start{0};
    uint16_t stop{0};
    uint16_t step{0};

    friend bool operator==(const Component&, const Component&) = default;
};

using Components = std::vector<Component>;

struct Spec {
    SelectedWeekdays weekdays{"1111111"};
    Components years;
    Components months;
    Components days;
    Components hours;
    Components minutes;
    Components seconds;
};

struct RecurrenceSpec {
    SelectedWeekdays weekdays;
    SelectedYears years;
    SelectedMonths months;
    SelectedDays days;
    SelectedHours hours;
    SelectedMinutes minutes;
    SelectedSeconds seconds;

    RecurrenceSpec(const Spec& spec);
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const Component& component);

auto findNextRecurrence(const RecurrenceSpec& spec, dw::DateTime refTimeStamp)
    -> Maybe<dw::DateTime>;

auto component_to_str(const Component& component) -> std::string;

template <typename Selectable>
auto fillSelectable(const Components& cs) -> Selectable;

auto makeYears(const Components& components) -> std::vector<int>;

auto weekdaysToString(const SelectedWeekdays& weekdays) -> std::string;

auto join_components(const auto& components) -> std::string;

auto parseComponents(Pattern pattern) -> Components;

auto parseComponent(std::string_view pattern) -> Component;

auto parseWeekdays(std::string_view daysPattern) -> SelectedWeekdays;

auto parseGroup(Pattern pattern, char delimiter) -> std::vector<Components>;

auto validate(const Components& components, const IntRange& intRange) -> bool;

auto validateSpec(const Spec& spec) -> void;

auto contains(Pattern pattern, char ch) -> bool;

constexpr auto operator<(Month lhs, Month rhs) noexcept -> bool;

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const Spec& spec);

class SpecParser {
public:
    auto operator()(Pattern pattern) -> Spec;

private:
    Spec spec;
    std::bitset<3> parsedParts;

    auto parse(Pattern pattern) -> void;

    auto parseTime(Pattern pattern) -> void;

    auto parseDate(Pattern pattern) -> void;
};

} // namespace

namespace sprint_timer {

Recurrence::Recurrence(std::string recPattern_)
{
    const auto spec = SpecParser{}(recPattern_);
    std::stringstream ss;
    ss << spec;
    recPattern = ss.str();
}

auto Recurrence::nextRecurrence(dw::DateTime referenceTimeStamp) const
    -> std::optional<dw::DateTime>
{
    const auto spec = SpecParser{}(recPattern);
    const auto recurrenceSpec = RecurrenceSpec{spec};
    return findNextRecurrence(spec, referenceTimeStamp);
}

auto Recurrence::pattern() const -> std::string { return recPattern; }

} // namespace sprint_timer

namespace {

auto findNextRecurrence(const RecurrenceSpec& spec, dw::DateTime refTimeStamp)
    -> Maybe<dw::DateTime>
{
    const auto startYear = static_cast<int>(refTimeStamp.year());

    auto years = spec.years;
    if (years.empty()) {
        std::ranges::copy(std::views::iota(startYear, startYear + 11),
                          std::back_inserter(years));
    }

    auto suitableYear = [startYear](auto x) { return x >= startYear; };

    auto suitableMonth = [&](auto x) {
        return spec.months.test(x) and
               !(x == startYear and Month{x} < refTimeStamp.month());
    };

    auto suitableDate = [&](auto date) {
        return date.valid() and date >= refTimeStamp.date() and
               spec.weekdays.test(static_cast<unsigned>(weekday(date)));
    };

    auto suitableHour = [&](auto x) {

    };

    using namespace std::chrono;
    using std::views::cartesian_product;
    using std::views::filter;
    using std::views::transform;

    // clang-format off
    for (auto y : years | filter(suitableYear)) {
        for (auto m : validMonthRange.valuesView() | filter(suitableMonth)) {
            for (auto date : validDayRange.valuesView()
                    | filter([&](auto x) { return spec.days.test(x); })
                    | std::views::transform([&](auto x) {
                              return Date{Year{y}, Month{m}, Day{x}};
                          })
                    | filter(suitableDate)) {
                for (auto h : validHourRange.valuesView() 
                    | filter([&](auto x) { return spec.hours.test(x); })
                    | transform([&](auto x) { return DateTime{date} + hours{x}; })
                    | filter([&](auto x) {
                        return x >= DateTime{refTimeStamp.date()} + refTimeStamp.hour();
                        })) {
                    for (auto min : validMinuteRange.valuesView()
                        | filter([&](auto x) { return spec.minutes.test(x); })
                        | transform([&](auto x) {
                            return h + minutes{x};
                            })
                        | filter([&](auto x) {
                            return x >= DateTime{refTimeStamp.date()} + refTimeStamp.hour() + refTimeStamp.minute();
                            })) {
                        for (auto s : validSecondRange.valuesView()
                            | filter([&](auto x) { 
                                  return spec.seconds.test(x);
                              })
                            | transform([&](auto x) {
                                  return min + seconds{x}; 
                              })) {
                            if (s >= refTimeStamp) {
                                return s;
                            }
                        }
                    }
                }
            }
        }
    }
    // clang-format on

    return std::nullopt;
}

template <typename Selectable>
auto fillSelectable(const Components& cs) -> Selectable
{
    Selectable selectable;

    if (cs.empty()) {
        selectable.set();
        return selectable;
    }

    for (const auto& component : cs) {
        std::cout << component_to_str(component) << std::endl;
        if (component.start == component.stop) {
            selectable.set(component.start);
            continue;
        }
        for (auto start = component.start; start <= component.stop;
             start += component.step) {
            selectable.set(start);
        }
    }

    return selectable;
}

auto component_to_str(const Component& component) -> std::string
{
    const auto [start, stop, step] = component;
    std::string res{std::format("{:0>2d}", start)};
    if (component.stop != component.start) {
        res += std::format("..{:0>2d}", stop);
    }
    if (component.step > 1) {
        res += std::format("/{}", step);
    }
    return res;
}

auto join_components(const auto& components) -> std::string
{
    return sprint_timer::utils::transformJoin(
        cbegin(components), cend(components), ",", component_to_str);
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const Spec& spec)
{
    os << weekdaysToString(spec.weekdays);

    os << (spec.years.empty() ? "*" : join_components(spec.years)) << "-";
    os << (spec.months.empty() ? "*" : join_components(spec.months)) << "-";
    os << (spec.days.empty() ? "*" : join_components(spec.days)) << " ";

    os << (spec.hours.empty() ? "*" : join_components(spec.hours)) << ":";
    os << (spec.minutes.empty() ? "*" : join_components(spec.minutes)) << ":";
    os << (spec.seconds.empty() ? "*" : join_components(spec.seconds));

    return os;
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const Component& component)
{
    os << "Component {" << component.start << ", " << component.stop << ", "
       << component.step << "}";
    return os;
}

auto parseWeekdays(Pattern daysPattern) -> SelectedWeekdays
{
    if (daysPattern.ends_with(",")) {
        daysPattern.remove_suffix(1);
    }

    SelectedWeekdays result;

    for (auto part : split(daysPattern, ',')) {
        const auto p = split(part, "..");
        auto start = std::ranges::find(dayNames, p.front());
        auto stop =
            p.size() == 2 ? std::ranges::find(dayNames, p.back()) : start;
        if (start == cend(dayNames) || stop == cend(dayNames) || p.size() > 2) {
            throw sprint_timer::DomainException{std::format(
                "Error parsing days in the pattern: {}", daysPattern)};
        }
        for (auto i :
             std::views::iota(std::distance(cbegin(dayNames), start),
                              std::distance(cbegin(dayNames), stop) + 1)) {
            result.set(static_cast<size_t>(i));
        }
    }

    return result;
}

auto parseComponent(std::string_view pattern) -> Component
{
    auto parts = split(pattern, '/');
    auto p = split(parts.front(), "..");

    auto start = std::stoul(std::string{p.front()});
    auto stop = p.size() == 2 ? std::stoul(std::string{p.back()}) : start;
    auto step = parts.size() == 2 ? std::stoul(std::string{parts.back()})
                : p.size() == 2   ? 1
                                  : 0;

    // normalize stop, so the total length is a multiple of step
    if (stop > start and step > 0) {
        stop -= (stop - start) % step;
    }

    return Component{static_cast<uint16_t>(start),
                     static_cast<uint16_t>(stop),
                     static_cast<uint16_t>(step)};
}

auto parseComponents(std::string_view pattern) -> Components
{
    Components components;
    if (pattern == "*") {
        return components;
    }

    auto parts = split(pattern, ',');
    components.reserve(parts.size());

    // return std::views::transform(split(pattern, ','), parseComponents) |
    //        std::ranges::to<std::vector>();
    std::ranges::copy(std::views::transform(parts, parseComponent),
                      std::back_inserter(components));

    return components;
}

auto parseGroup(Pattern pattern, char delimiter) -> std::vector<Components>
{
    auto splitted = split(pattern, delimiter);
    auto transformed = std::views::transform(splitted, parseComponents);
    // std::vector<Components> result =
    // std::ranges::to<std::vector>(transformed);
    std::vector<Components> result;
    std::ranges::copy(transformed, std::back_inserter(result));
    std::ranges::for_each(result, [](auto& components) {
        std::ranges::sort(components, [](const auto& left, const auto& right) {
            return left.start < right.start;
        });
        auto last = std::unique(begin(components), end(components));
        components.erase(last, end(components));
    });
    return result;
}

auto validateSpec(const Spec& spec) -> void
{
    validate(spec.years, validYearRange);
    validate(spec.months, validMonthRange);
    validate(spec.days, validDayRange);
    validate(spec.hours, validHourRange);
    validate(spec.minutes, validMinuteRange);
    validate(spec.seconds, validSecondRange);
}

auto validate(const Components& components, const IntRange& intRange) -> bool
{
    for (const auto& component : components) {
        const auto [start, stop, step] = component;
        if (not intRange(start) || step > intRange.max - intRange.min ||
            not intRange(stop)) {
            throw std::runtime_error{component_to_str(component)};
        }
        // if (not intRange(start) || step > intRange.max - intRange.min ||
        //     not intRange(stop) || start + step > stop) {
        //     throw std::runtime_error{component_to_str(component)};
        // }
    }
    return true;
}

auto weekdaysToString(const SelectedWeekdays& weekdays) -> std::string
{
    std::string res;

    if (weekdays.all()) {
        return res;
    }

    for (size_t start = 0; start < weekdays.size(); ++start) {
        if (not weekdays.test(start)) {
            continue;
        }

        res += std::format("{}", dayNames[start]);

        size_t stop = start + 1;
        for (; stop < weekdays.size() and weekdays.test(stop); ++stop)
            ;
        if (stop - start > 2) {
            res += std::format("..{}", dayNames[stop - 1]);
            start = stop;
        }

        res.push_back(',');
    }

    if (res.back() == ',') {
        res.pop_back();
    }
    res.push_back(' ');

    return res;
}

auto makeYears(const Components& components) -> std::vector<int>
{
    SelectedYears selectedYears;
    for (const auto& component : components) {
        if (component.start == component.stop) {
            selectedYears.push_back(component.start);
            continue;
        }
        for (auto start = component.start; start <= component.stop;
             start += component.step) {
            selectedYears.push_back(start);
        }
    }
    return selectedYears;
}

RecurrenceSpec::RecurrenceSpec(const Spec& spec)
    : weekdays{spec.weekdays}
    , years{makeYears(spec.years)}
    , months{fillSelectable<SelectedMonths>(spec.months)}
    , days{fillSelectable<SelectedDays>(spec.days)}
    , hours{fillSelectable<SelectedHours>(spec.hours)}
    , minutes{fillSelectable<SelectedMinutes>(spec.minutes)}
    , seconds{fillSelectable<SelectedSeconds>(spec.seconds)}
{
}

auto SpecParser::operator()(Pattern pattern) -> Spec
{
    spec = Spec{};
    parsedParts.reset();

    static constexpr auto dict =
        Dictionary<std::string_view, std::string_view, shorthands.size()>{
            {shorthands}};

    // Check if any shourhand name is used, like "daily" and substitute it
    // with proper expression.
    if (dict.contains(pattern)) {
        pattern = dict.at(pattern);
    }

    auto parts = split(pattern, ' ');
    std::ranges::for_each(parts, [this](const auto& p) { parse(p); });

    // If time is omitted, set it to 00:00:00
    if (not parsedParts.test(timePos)) {
        spec.hours = Components{Component{}};
        spec.minutes = Components{Component{}};
        spec.seconds = Components{Component{}};
    }

    try {
        validateSpec(spec);
        return spec;
    }
    catch (std::exception& exc) {
        std::stringstream ss;
        ss << spec;
        throw sprint_timer::DomainException{
            std::format("Validation failed for spec: '{}' for part '{}'",
                        ss.str(),
                        exc.what())};
    }
}

auto SpecParser::parse(Pattern pattern) -> void
{
    if (contains(pattern, ':')) {
        if (parsedParts.test(timePos)) {
            throw std::runtime_error{"Multiple time entries"};
        }
        parseTime(pattern);
        parsedParts.set(timePos);
    }
    else if (contains(pattern, '-')) {
        if (parsedParts.test(datePos)) {
            throw std::runtime_error{"Multiple date entries"};
        }
        parseDate(pattern);
        parsedParts.set(datePos);
    }
    else {
        if (parsedParts.test(weekdayPos)) {
            throw std::runtime_error{"Multiple weekday entries"};
        }
        spec.weekdays = parseWeekdays(pattern);
        parsedParts.set(weekdayPos);
    }
}

auto SpecParser::parseTime(Pattern pattern) -> void
{
    auto transformed = parseGroup(pattern, ':');
    spec.hours = transformed[0];
    spec.minutes = transformed[1];
    spec.seconds = transformed.size() == 3 ? transformed[2]
                                           : Components{Component{0, 0, 0}};
}

auto SpecParser::parseDate(Pattern pattern) -> void
{
    auto transformed = parseGroup(pattern, '-');
    if (transformed.size() == 3) {
        auto fixYear = [](auto year) -> uint16_t {
            if (year >= 0 and year < 70) {
                return year + 2000;
            }
            if (year >= 70 and year < 100) {
                return year + 1900;
            }
            return year;
        };
        auto fixComponentYear = [&](auto& component) {
            component.start = fixYear(component.start);
            component.stop = fixYear(component.stop);
        };
        std::ranges::for_each(transformed[0], fixComponentYear);
        spec.years = transformed[0];
        spec.months = transformed[1];
        spec.days = transformed[2];
    }
    else {
        spec.months = transformed[0];
        spec.days = transformed[1];
    }
}

auto contains(Pattern pattern, char ch) -> bool
{
    return pattern.find(ch) != pattern.npos;
}

constexpr auto operator<(Month lhs, Month rhs) noexcept -> bool
{
    return unsigned(lhs) < unsigned(rhs);
}

} // namespace
