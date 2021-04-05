/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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

    MainWindow& operator=(const MainWindow&) = delete;

    MainWindow(MainWindow&&) = delete;

    MainWindow& operator=(MainWindow&&) = delete;

    QSize sizeHint() const override;

private:
    struct ExpandedOutlines {
        explicit ExpandedOutlines(MainWindow& widget);
    };
    struct Shrinked {
        explicit Shrinked(MainWindow& widget);
    };
    struct ExpandedMenu {
        explicit ExpandedMenu(MainWindow& widget);
    };
    struct Expanded {
        explicit Expanded(MainWindow& widget);
    };

    using State = std::variant<std::monostate,
                               ExpandedOutlines,
                               ExpandedMenu,
                               Expanded,
                               Shrinked>;

    struct ViewToggledEvent {
        MainWindow& widget;

        explicit ViewToggledEvent(MainWindow& widget);

        State operator()(std::monostate);
        State operator()(const ExpandedOutlines&);
        State operator()(const Shrinked&);
        State operator()(const ExpandedMenu&);
        State operator()(const Expanded&);
    };

    struct MenuToggledEvent {
        MainWindow& widget;

        explicit MenuToggledEvent(MainWindow& widget);

        State operator()(std::monostate);
        State operator()(const ExpandedOutlines&);
        State operator()(const Shrinked&);
        State operator()(const ExpandedMenu&);
        State operator()(const Expanded&);
    };

    std::unique_ptr<Ui::MainWindow> ui;
    QWidget* sprintsWidget;
    QWidget* tasksWidget;
    QWidget* menuWidget;
    State state_;
    QSize size;

private slots:
    void toggleView();
    void toggleMenu();
};

} // namespace sprint_timer::ui::qt_gui


#endif // MAINWINDOW_H
