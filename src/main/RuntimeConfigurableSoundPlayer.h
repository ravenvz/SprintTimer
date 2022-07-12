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
#ifndef RUNTIMECONFIGURABLESOUNDPLAYER_H_N5HGISEB
#define RUNTIMECONFIGURABLESOUNDPLAYER_H_N5HGISEB

#include "api/IConfig.h"
#include "core/CompositionObserver.h"
#include "api/SoundPlayer.h"

namespace sprint_timer::compose {

class RuntimeConfigurableSoundPlayer : public api::SoundPlayer {
public:
    RuntimeConfigurableSoundPlayer(api::IConfig& applicationSettings_,
                                   Observable& configChangedSignaller_,
                                   std::unique_ptr<api::SoundPlayer> wrapped_)
        : applicationSettings{applicationSettings_}
        , wrapped{std::move(wrapped_)}
        , observer{configChangedSignaller_, [this]() { onConfigChanged(); }}
    {
    }

    void setVolume(int volume) override { wrapped->setVolume(volume); }

    void play(const std::string& mediaPath) override
    {
        wrapped->play(mediaPath);
    }

    void setSoundEnabled(bool enabled) override
    {
        wrapped->setSoundEnabled(enabled);
    }

    void onConfigChanged()
    {
        wrapped->setVolume(applicationSettings.soundVolume());
        wrapped->setSoundEnabled(applicationSettings.soundIsEnabled());
    }

private:
    api::IConfig& applicationSettings;
    std::unique_ptr<api::SoundPlayer> wrapped;
    CompositionObserver observer;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: RUNTIMECONFIGURABLESOUNDPLAYER_H_N5HGISEB */
