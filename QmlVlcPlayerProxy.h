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

#ifndef QMLVLCPLAYERPROXY_H
#define QMLVLCPLAYERPROXY_H

#include <QObject>
#include <QTimer>

#include "QmlVlcVideoOutput.h"
#include "QmlVlcAudio.h"
#include "QmlVlcInput.h"
#include "QmlVlcPlaylist.h"
#include "QmlVlcSubtitle.h"
#include "QmlVlcVideo.h"
#include "QmlVlcMedia.h"

class QmlVlcPlayerProxy
    : public QObject,
      public QQmlParserStatus,
      private vlc::media_player_events_callback
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

protected:
    virtual void classBegin() override;
    virtual void componentComplete() override;
    void classEnd();

private:
    void media_player_event( const libvlc_event_t* e ) override;

public:
    explicit QmlVlcPlayerProxy( vlc::player* player, QObject* parent = 0 );
    ~QmlVlcPlayerProxy();

    Q_PROPERTY( QString vlcVersion READ get_vlcVersion )

    Q_PROPERTY( bool playing READ get_playing NOTIFY playingChanged )
    Q_PROPERTY( double length READ get_length NOTIFY mediaPlayerLengthChanged )

    enum State {
        NothingSpecial = ::libvlc_NothingSpecial,
        Opening = ::libvlc_Opening,
        Buffering = ::libvlc_Buffering,
        Playing = ::libvlc_Playing,
        Paused = ::libvlc_Paused,
        Stopped = ::libvlc_Stopped,
        Ended = ::libvlc_Ended,
        Error = ::libvlc_Error,
    };
    Q_ENUMS( State )
    Q_PROPERTY( State state READ get_state NOTIFY stateChanged )

    Q_PROPERTY( QString mrl READ get_mrl WRITE set_mrl )
    Q_PROPERTY( double position READ get_position WRITE set_position NOTIFY mediaPlayerPositionChanged )
    Q_PROPERTY( double time READ get_time WRITE set_time NOTIFY mediaPlayerTimeChanged )
    Q_PROPERTY( unsigned volume READ get_volume WRITE set_volume )

    Q_PROPERTY( QmlVlcInput* input READ get_input CONSTANT )
    Q_PROPERTY( QmlVlcVideo* video READ get_video CONSTANT )
    Q_PROPERTY( QmlVlcAudio* audio READ get_audio CONSTANT )
    Q_PROPERTY( QmlVlcSubtitle* subtitle READ get_subtitle CONSTANT )
    Q_PROPERTY( QmlVlcPlaylist* playlist READ get_playlist CONSTANT )
    Q_PROPERTY( QmlVlcMedia* mediaDescription READ get_mediaDesc CONSTANT )

Q_SIGNALS:
    void playingChanged();
    void stateChanged();

public:
    //QML Api
    QString get_vlcVersion();

    Q_INVOKABLE void play( const QString& mrl );
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void togglePause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void mute();
    Q_INVOKABLE void unMute();
    Q_INVOKABLE void toggleMute();

    QString get_mrl();
    void set_mrl( const QString& mrl );

    bool get_playing();

    double get_length();

    double get_position();
    void set_position( double );

    double get_time();
    void set_time( double );

    unsigned int get_volume();
    void set_volume( unsigned int );

    State get_state();

    vlc::player& player()
        { assert( m_player ); return *m_player; }

Q_SIGNALS:
    /* async events from libvlc */
    void mediaPlayerMediaChanged();
    void mediaPlayerNothingSpecial();
    void mediaPlayerOpening();
    void mediaPlayerBuffering( float percents );
    void mediaPlayerPlaying();
    void mediaPlayerPaused();
    void mediaPlayerForward();
    void mediaPlayerBackward();
    void mediaPlayerEncounteredError();
    void mediaPlayerEndReached();
    void mediaPlayerStopped();
    void mediaPlayerTitleChanged();

    void mediaPlayerTimeChanged( double time );
    void mediaPlayerPositionChanged( float position );
    void mediaPlayerSeekableChanged( bool seekable );
    void mediaPlayerPausableChanged( bool pausable );
    void mediaPlayerLengthChanged( double length );

private Q_SLOTS:
    void currentItemEndReached();

public:
    QmlVlcAudio*     get_audio()     { return &m_audio; }
    QmlVlcInput*     get_input()     { return &m_input; }
    QmlVlcPlaylist*  get_playlist()  { return &m_playlist; }
    QmlVlcSubtitle*  get_subtitle()  { return &m_subtitle; }
    QmlVlcVideo*     get_video()     { return &m_video; }
    QmlVlcMedia*     get_mediaDesc() { return &m_currentMediaDesc; }

private:
    QmlVlcAudio        m_audio;
    QmlVlcInput        m_input;
    QmlVlcPlaylist     m_playlist;
    QmlVlcSubtitle     m_subtitle;
    QmlVlcVideo        m_video;
    QmlVlcCurrentMedia m_currentMediaDesc;

private:
    vlc::player* m_player;
    QTimer m_errorTimer;
};
#endif // QMLVLCPLAYERPROXY_H
