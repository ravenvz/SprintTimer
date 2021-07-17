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
#ifndef CONFIGURABLEASSETLIBRARY_H_1PX4JSGU
#define CONFIGURABLEASSETLIBRARY_H_1PX4JSGU

#include "qt_gui/presentation/AssetLibrary.h"
#include <vector>

namespace sprint_timer::ui {

class ConfigurableAssetLibrary : public AssetLibrary {
public:
    explicit ConfigurableAssetLibrary(
        const std::vector<std::pair<std::string, std::string>>& entries);

    std::optional<std::string>
    filePath(const std::string& assetId) const override;

    void replaceAssetPath(const std::pair<std::string, std::string>& entry);

private:
    std::unordered_map<std::string, std::string> assetPathMapper;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: CONFIGURABLEASSETLIBRARY_H_1PX4JSGU */
