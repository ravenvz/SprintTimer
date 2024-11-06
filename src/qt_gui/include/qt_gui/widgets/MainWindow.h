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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cpp_utils/patterns/State.h"
#include <QWidget>
#include <memory>
#include <variant>
#include <vector>

namespace Ui {
class MainWindow;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

class MainWindow : public QWidget {

public:
    MainWindow(std::unique_ptr<QWidget> sprintOutline,
               std::unique_ptr<QWidget> taskOutline,
               std::unique_ptr<QWidget> todayProgressIndicator,
               std::unique_ptr<QWidget> timerWidget,
               std::unique_ptr<QWidget> launcherMenu,
               QWidget* parent = nullptr);

    ~MainWindow() override;

    MainWindow(const MainWindow&) = delete;

    auto operator=(const MainWindow&) -> MainWindow& = delete;

    MainWindow(MainWindow&&) = delete;

    auto operator=(MainWindow&&) -> MainWindow& = delete;

    [[nodiscard]] auto sizeHint() const -> QSize override;

private:
    class BaseState {
    protected:
        explicit BaseState(std::reference_wrapper<MainWindow> context_)
            : context{context_}
        {
        }

        std::reference_wrapper<MainWindow> context;
    };

    class Setup : BaseState {
    public:
        using BaseState::context;

        explicit Setup(std::reference_wrapper<MainWindow> ctx)
            : BaseState{ctx}
        {
        }
    };

    class ExpandedOutlines : BaseState {
    public:
        using BaseState::context;

        explicit ExpandedOutlines(std::reference_wrapper<MainWindow> ctx)
            : BaseState{ctx}
        {
            context.get().expandOutline();
        }
    };

    class Shrinked : BaseState {
    public:
        using BaseState::context;

        explicit Shrinked(std::reference_wrapper<MainWindow> ctx)
            : BaseState{ctx}
        {
            context.get().shrink();
        }
    };

    class Expanded : BaseState {
    public:
        using BaseState::context;

        explicit Expanded(std::reference_wrapper<MainWindow> ctx)
            : BaseState{ctx}
        {
            context.get().expand();
        }
    };

    class ExpandedMenu : BaseState {
    public:
        using BaseState::context;

        explicit ExpandedMenu(std::reference_wrapper<MainWindow> ctx)
            : BaseState{ctx}
        {
            context.get().expandMenu();
        }
    };

    using State =
        std::variant<Setup, Expanded, Shrinked, ExpandedOutlines, ExpandedMenu>;

    using MaybeState = std::optional<State>;

    struct ExpandButtonToggled { };

    struct ExpandMenuButtonToggled { };

    class TransitionTable {
    public:
        auto operator()(Setup& state,
                        ExpandButtonToggled /*unused*/) -> MaybeState
        {
            return ExpandedOutlines{state.context};
        }

        auto operator()(Setup& state,
                        ExpandMenuButtonToggled /*unused*/) -> MaybeState
        {
            return ExpandedMenu{state.context};
        }

        auto operator()(Shrinked& state,
                        ExpandButtonToggled /*unused*/) -> MaybeState
        {
            return ExpandedOutlines{state.context};
        }

        auto operator()(Shrinked& state,
                        ExpandMenuButtonToggled /*unused*/) -> MaybeState
        {
            return ExpandedMenu{state.context};
        }

        auto operator()(ExpandedOutlines& state,
                        ExpandButtonToggled /*unused*/) -> MaybeState
        {
            return Shrinked{state.context};
        }

        auto operator()(ExpandedOutlines& state,
                        ExpandMenuButtonToggled /*unused*/) -> MaybeState
        {
            return Expanded{state.context};
        }

        auto operator()(ExpandedMenu& state,
                        ExpandButtonToggled /*unused*/) -> MaybeState
        {
            return Expanded{state.context};
        }

        auto operator()(ExpandedMenu& state,
                        ExpandMenuButtonToggled /*unused*/) -> MaybeState
        {
            return Shrinked{state.context};
        }

        auto operator()(Expanded& state,
                        ExpandButtonToggled /*unused*/) -> MaybeState
        {
            return ExpandedMenu{state.context};
        }

        auto operator()(Expanded& state,
                        ExpandMenuButtonToggled /*unused*/) -> MaybeState
        {
            return ExpandedOutlines{state.context};
        }
    };

    auto expand() -> void;

    auto expandMenu() -> void;

    auto expandOutline() -> void;

    auto shrink() -> void;

    std::unique_ptr<Ui::MainWindow> ui;
    QWidget* sprintsWidget;
    QWidget* tasksWidget;
    QWidget* menuWidget;
    QSize size;
    patterns::FSMExternalTransitions<TransitionTable,
                                     Setup,
                                     Expanded,
                                     Shrinked,
                                     ExpandedOutlines,
                                     ExpandedMenu>
        fsm;

private slots:
    void toggleView();
    void toggleMenu();
};

} // namespace sprint_timer::ui::qt_gui

#endif // MAINWINDOW_H
