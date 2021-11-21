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
#include "qt_gui/QtSoundPlayerImp.h"
#include <QMediaPlayer>

namespace sprint_timer::ui::qt_gui {

QtSoundPlayerImp::QtSoundPlayerImp(QMediaPlayer& qmediaPlayer_)
    : qmediaPlayer{qmediaPlayer_}
{
}

void QtSoundPlayerImp::setVolume(int volume)
{
    audioOut.setVolume(static_cast<float>(volume) / 100.0f);
}

void QtSoundPlayerImp::play(const std::string& mediaPath)
{
    // That's ok as long as we do not play, say videos, which is out of
    // scope of this application
    if (!soundEnabled) {
        return;
    }
    qmediaPlayer.setAudioOutput(&audioOut);
    qmediaPlayer.setSource(
        QUrl::fromLocalFile(QString::fromStdString(mediaPath)));
    qmediaPlayer.play();
}

void QtSoundPlayerImp::setSoundEnabled(bool enabled) { soundEnabled = enabled; }

} // namespace sprint_timer::ui::qt_gui
