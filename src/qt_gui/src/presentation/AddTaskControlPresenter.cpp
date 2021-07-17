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
#include "qt_gui/presentation/AddTaskControlPresenter.h"
#include "core/utils/StringUtils.h"
#include <regex>

namespace {

constexpr std::string_view tagReg{"^#(\\w+)"};
constexpr std::string_view estimatedReg{"^\\*(\\w+)"};
constexpr std::string_view anyNonWhitespaceReg{"\\S+"};

std::tuple<std::string, std::vector<std::string>, int32_t>
decodeDescription(const std::string& description);

} // namespace

namespace sprint_timer::ui {

AddTaskControlPresenter::AddTaskControlPresenter(
    CommandHandler<use_cases::CreateTaskCommand>& createTaskHandler_)
    : createTaskHandler{createTaskHandler_}
{
}

void AddTaskControlPresenter::addTask(const use_cases::TaskDTO& details) const
{
    createTaskHandler.handle(use_cases::CreateTaskCommand{
        details.name, details.tags, details.expectedCost});
}

void AddTaskControlPresenter::addTask(
    const std::string& encodedDescription) const
{
    auto [name, tags, cost] = decodeDescription(encodedDescription);
    createTaskHandler.handle(use_cases::CreateTaskCommand{name, tags, cost});
}

} // namespace sprint_timer::ui

namespace  {

std::tuple<std::string, std::vector<std::string>, int32_t>
decodeDescription(const std::string& description)
{
    std::regex tagRegex{tagReg.cbegin(), tagReg.cend()};
    std::regex estimatedRegex{cbegin(estimatedReg), cend(estimatedReg)};
    std::regex anyNonWhitespace{anyNonWhitespaceReg.cbegin(),
                                anyNonWhitespaceReg.cend()};

    std::sregex_iterator words_begin{
        description.begin(), description.end(), anyNonWhitespace};
    std::sregex_iterator words_end;

    std::vector<std::string> nameParts;
    std::vector<std::string> tags;
    int32_t estimatedCost{1};

    for (auto it = words_begin; it != words_end; ++it) {
        std::string word{it->str()};
        if (std::regex_match(word, tagRegex)) {
            tags.push_back(word.substr(1));
        }
        else if (std::regex_match(word, estimatedRegex)) {
            estimatedCost = stoi(word.substr(1));
        }
        else {
            nameParts.push_back(word);
        }
    }

    std::string name = sprint_timer::utils::join(nameParts, " ");

    return {name, tags, estimatedCost};
}

} // namespace 
