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

#include "core/CommandHandler.h"
#include "core/SprintConflictException.h"
#include "core/SprintTimerException.h"
#include "qt_storage/QueryError.h"
#include <QMessageBox>
#include <sstream>
#include <string_view>

namespace sprint_timer {

template <typename CommandT>
class ErrorReportingCommandHandler : public CommandHandler<CommandT> {
public:
    ErrorReportingCommandHandler(
        std::unique_ptr<CommandHandler<CommandT>> wrapped);

    void handle(CommandT&& command) override;

private:
    std::unique_ptr<CommandHandler<CommandT>> wrapped;
};

template <typename CommandT>
ErrorReportingCommandHandler<CommandT>::ErrorReportingCommandHandler(
    std::unique_ptr<CommandHandler<CommandT>> wrapped_)
    : wrapped{std::move(wrapped_)}
{
}

template <typename CommandT>
void ErrorReportingCommandHandler<CommandT>::handle(CommandT&& command)
{
    try {
        wrapped->handle(std::move(command));
    }
    catch (storage::qt_storage::QueryError& exc) {
        std::stringstream ss;
        ss << exc.queryText() << '\n';
        ss << exc.queryError() << '\n';
        QMessageBox msgBox;
        msgBox.setText(QString::fromStdString(ss.str()));
        msgBox.exec();
    }
    catch (SprintConflictException& exc) {
        QMessageBox msgBox;
        msgBox.setText(exc.what());
        std::stringstream ss;
        ss << "Conflict detected between following pairs of sprints:\n";
        std::string_view dateFormat{"hh:mm:ss dd.MM.yyyy"};
        for (auto& [left, right] : exc.conflictingSprints()) {
            ss << dw::to_string(left.timeSpan(), dateFormat) << " and "
               << dw::to_string(right.timeSpan(), dateFormat) << '\n';
        }
        msgBox.setInformativeText(QString::fromStdString(ss.str()));
        msgBox.exec();
    }
    catch (SprintTimerException& exc) {
        QMessageBox msgBox;
        msgBox.setText("Exception when handling command");
        msgBox.setInformativeText(exc.what());
        msgBox.exec();
    }
    catch (std::exception& exc) {
        QMessageBox msgBox;
        std::stringstream ss;
        msgBox.setText("Exception when handling command");
        ss << command << " \n with description: " << exc.what();
        msgBox.setText(QString::fromStdString(ss.str()));
        msgBox.exec();
        throw;
    }
}

} // namespace sprint_timer
