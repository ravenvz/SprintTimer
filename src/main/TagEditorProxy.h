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
#ifndef TAGEDITORPROXY_H_R1NUUTME
#define TAGEDITORPROXY_H_R1NUUTME

#include "DisplayableLifestyleProxy.h"
#include <qt_gui/widgets/TagEditor.h>

namespace sprint_timer::compose {

class TagEditorProxy : public DisplayableLifestyleProxy {
public:
    explicit TagEditorProxy(QAbstractItemModel& tagModel);

private:
    QAbstractItemModel& tagModel;

    std::unique_ptr<ui::qt_gui::StandaloneDisplayableWidget> create() override;
};

inline TagEditorProxy::TagEditorProxy(QAbstractItemModel& tagModel_)
    : tagModel{tagModel_}
{
}

inline std::unique_ptr<ui::qt_gui::StandaloneDisplayableWidget>
TagEditorProxy::create()
{
    return std::make_unique<ui::qt_gui::TagEditor>(tagModel);
}

} // namespace sprint_timer::compose

#endif /* end of include guard: TAGEDITORPROXY_H_R1NUUTME */
