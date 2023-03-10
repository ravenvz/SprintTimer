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
#ifndef ACTION_H_LZE48PW5
#define ACTION_H_LZE48PW5

#include <memory>
#include <string>

namespace sprint_timer {

class Action {
public:
    template <typename ActionT>
    Action(ActionT action_)
        : pimpl{std::make_unique<ActionModel<ActionT>>(std::move(action_))}
    {
    }

    ~Action() = default;

    Action(const Action& other)
        : pimpl{other.pimpl->clone()}
    {
    }

    auto operator=(const Action& other) -> Action&
    {
        if (&other == this) {
            return *this;
        }
        other.pimpl->clone().swap(pimpl);
        return *this;
    }

    Action(Action&& other) = default;
    auto operator=(Action&& other) -> Action& = default;

    auto execute() -> void { pimpl->do_execute(); }

    auto undo() -> void { pimpl->do_undo(); }

    auto describe() const -> std::string { return pimpl->do_describe(); }

private:
    struct ActionConcept {
        virtual ~ActionConcept() = default;

        virtual auto do_execute() -> void = 0;

        virtual auto do_undo() -> void = 0;

        [[nodiscard]] virtual auto do_describe() const -> std::string = 0;

        virtual auto clone() -> std::unique_ptr<ActionConcept> = 0;
    };

    template <typename ActionT> struct ActionModel : public ActionConcept {
        explicit ActionModel(ActionT action_)
            : action{std::move(action_)}
        {
        }

        auto do_execute() -> void override { action.execute(); }

        auto do_undo() -> void override { action.undo(); }

        [[nodiscard]] auto do_describe() const -> std::string override
        {
            return action.describe();
        }

        auto clone() -> std::unique_ptr<ActionConcept> override
        {
            return std::make_unique<ActionModel>(*this);
        }

        ActionT action;
    };

    std::unique_ptr<ActionConcept> pimpl;
};

} // namespace sprint_timer

#endif /* end of include guard: ACTION_H_LZE48PW5 */

