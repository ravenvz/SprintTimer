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
#ifndef QTSOUNDPLAYERIMP_H_F7WYTSZA
#define QTSOUNDPLAYERIMP_H_F7WYTSZA

#include "core/SoundPlayer.h"
#include <QAudioOutput>
#include <QMediaPlayer>

namespace sprint_timer::ui::qt_gui {

class QtSoundPlayerImp : public SoundPlayer {
public:
    explicit QtSoundPlayerImp(QMediaPlayer& qmediaPlayer_)
        : qmediaPlayer{qmediaPlayer_}
    {
    }

    void setVolume(int volume) override
    {
        audioOut.setVolume(static_cast<float>(volume) / 100.0f);
    }

    void play(const std::string& mediaPath) override
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

    void setSoundEnabled(bool enabled) override { soundEnabled = enabled; }

private:
    QAudioOutput audioOut;
    QMediaPlayer& qmediaPlayer;
    bool soundEnabled{true};
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: QTSOUNDPLAYERIMP_H_F7WYTSZA */
