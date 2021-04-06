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
#ifndef SETTINGSWATCHINGASSETLIBRARY_H_Z2VS0DMQ
#define SETTINGSWATCHINGASSETLIBRARY_H_Z2VS0DMQ

#include <core/CompositionObserver.h>
#include <core/IConfig.h>
#include <qt_gui/presentation/ConfigurableAssetLibrary.h>

namespace sprint_timer::compose {

class SettingsWatchingAssetLibrary : public AssetLibrary {
public:
    SettingsWatchingAssetLibrary(ui::ConfigurableAssetLibrary& wrapped,
                                 const IConfig& settings,
                                 Observable& configChangedSignaller);

    std::optional<std::string>
    filePath(const std::string& assetId) const override;

    void onConfigChanged();

private:
    ui::ConfigurableAssetLibrary& wrapped;
    const IConfig& settings;
    CompositionObserver configChangedWatcher;
    std::string cachedRingSoundFilePath{settings.soundFilePath()};
};

inline SettingsWatchingAssetLibrary::SettingsWatchingAssetLibrary(
    ui::ConfigurableAssetLibrary& wrapped_,
    const IConfig& settings_,
    Observable& configChangedSignaller_)
    : wrapped{wrapped_}
    , settings{settings_}
    , configChangedWatcher{configChangedSignaller_,
                           [this]() { onConfigChanged(); }}
{
}

inline std::optional<std::string>
SettingsWatchingAssetLibrary::filePath(const std::string& assetId) const
{
    return wrapped.filePath(assetId);
}

inline void SettingsWatchingAssetLibrary::onConfigChanged()
{
    if (auto soundPath = settings.soundFilePath();
        soundPath != cachedRingSoundFilePath) {
        cachedRingSoundFilePath = soundPath;
        wrapped.replaceAssetPath({"ringSound", soundPath});
    }
}

} // namespace sprint_timer::compose

#endif /* end of include guard: SETTINGSWATCHINGASSETLIBRARY_H_Z2VS0DMQ */
