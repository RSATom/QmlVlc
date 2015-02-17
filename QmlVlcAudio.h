/*******************************************************************************
* Copyright © 2014-2015, Sergey Radionov <rsatom_gmail.com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.

* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#ifndef QMLVLCAUDIO_H
#define QMLVLCAUDIO_H

#include <QObject>

#include "libvlc_wrapper/vlc_player.h"

class QmlVlcAudio : public QObject
{
    Q_OBJECT
public:
    QmlVlcAudio( vlc::player& player )
        : m_player( player ) { }

    enum Output {
        Stereo = ::libvlc_AudioChannel_Stereo,
        ReverseStereo = ::libvlc_AudioChannel_RStereo,
        Left = ::libvlc_AudioChannel_Left,
        Right = ::libvlc_AudioChannel_Right,
        Dolby = ::libvlc_AudioChannel_Dolbys,
    };
    Q_ENUMS( Output )

    Q_PROPERTY( unsigned count READ get_trackCount )

    Q_PROPERTY( int track READ get_track WRITE set_track )
    Q_PROPERTY( bool mute READ get_mute WRITE set_mute )
    Q_PROPERTY( unsigned volume READ get_volume WRITE set_volume )
    Q_PROPERTY( Output channel READ get_channel WRITE set_channel )
    Q_PROPERTY( int delay READ get_delay WRITE set_delay )

    unsigned get_trackCount();

    bool get_mute();
    void set_mute( bool );

    unsigned get_volume();
    void set_volume( unsigned );

    int get_track();
    void set_track( int idx );

    Output get_channel();
    void set_channel( Output );

    //in milliseconds
    int get_delay();
    void set_delay( int );

    Q_INVOKABLE void toggleMute();

    Q_INVOKABLE QString description( unsigned idx);

private:
    vlc::player& m_player;
};

#endif //QMLVLCAUDIO_H
