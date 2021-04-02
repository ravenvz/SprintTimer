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
#ifndef QTSOUNDPLAYERIMP_H_F7WYTSZA
#define QTSOUNDPLAYERIMP_H_F7WYTSZA

#include <QMediaPlayer>
#include <core/SoundPlayer.h>

namespace sprint_timer::qt_gui {

class SoundPlayerImp : public SoundPlayer {
public:
    explicit SoundPlayerImp(QMediaPlayer& qmediaPlayer_)
        : qmediaPlayer{qmediaPlayer_}
    {
    }

    void setVolume(int volume) override { qmediaPlayer.setVolume(volume); }

    void play(const std::string& mediaPath) override
    {
        // That's ok as long as we do not play, say videos, which is out of
        // scope of this application
        if (!soundEnabled) {
            return;
        }
        qmediaPlayer.setMedia(
            QUrl::fromLocalFile(QString::fromStdString(mediaPath)));
        qmediaPlayer.play();
    }

    void setSoundEnabled(bool enabled) override { soundEnabled = enabled; }

private:
    QMediaPlayer& qmediaPlayer;
    bool soundEnabled{true};
};

} // namespace sprint_timer::qt_gui

#endif /* end of include guard: QTSOUNDPLAYERIMP_H_F7WYTSZA */
