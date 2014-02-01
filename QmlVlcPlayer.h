#ifndef QMLVLCPLAYER_H
#define QMLVLCPLAYER_H

#include "QmlVlcVideoOutput.h"
#include "QmlVlcAudio.h"
#include "QmlVlcInput.h"
#include "QmlVlcPlaylist.h"
#include "QmlVlcSubtitle.h"
#include "QmlVlcVideo.h"
#include "QmlVlcMediaDesc.h"

class QmlVlcPlayer : public QmlVlcVideoOutput
{
    Q_OBJECT
public:
    explicit QmlVlcPlayer( vlc::player* player, QObject* parent = 0 );

    Q_PROPERTY( QString version READ get_version )
    Q_PROPERTY( QString vlcVersion READ get_vlcVersion )
    Q_PROPERTY( bool playing READ get_playing NOTIFY playingChanged )
    Q_PROPERTY( double length READ get_length NOTIFY MediaPlayerMediaChanged);
    Q_PROPERTY( double position READ get_position WRITE set_position NOTIFY MediaPlayerPositionChanged );
    Q_PROPERTY( double time READ get_time WRITE set_time NOTIFY MediaPlayerTimeChanged );
    Q_PROPERTY( unsigned volume READ get_volume WRITE set_volume );
    Q_PROPERTY( bool fullscreen READ get_fullscreen WRITE set_fullscreen );
    Q_PROPERTY( int state READ get_state NOTIFY stateChanged );

    Q_PROPERTY( int libvlc_NothingSpecial MEMBER libvlc_NothingSpecial CONSTANT );
    Q_PROPERTY( int libvlc_Opening MEMBER libvlc_Opening CONSTANT );
    Q_PROPERTY( int libvlc_Buffering MEMBER libvlc_Buffering CONSTANT );
    Q_PROPERTY( int libvlc_Playing MEMBER libvlc_Playing CONSTANT );
    Q_PROPERTY( int libvlc_Paused MEMBER libvlc_Paused CONSTANT );
    Q_PROPERTY( int libvlc_Stopped MEMBER libvlc_Stopped CONSTANT );
    Q_PROPERTY( int libvlc_Ended MEMBER libvlc_Ended CONSTANT );
    Q_PROPERTY( int libvlc_Error MEMBER libvlc_Error CONSTANT );

    Q_PROPERTY( QmlVlcAudio* audio READ get_audio CONSTANT );
    Q_PROPERTY( QmlVlcInput* input READ get_input CONSTANT )
    Q_PROPERTY( QmlVlcPlaylist* playlist READ get_playlist CONSTANT );
    Q_PROPERTY( QmlVlcSubtitle* subtitle READ get_subtitle CONSTANT );
    Q_PROPERTY( QmlVlcVideo* video READ get_video CONSTANT );
    Q_PROPERTY( QmlVlcMediaDesc* mediaDesc READ get_mediaDesc CONSTANT );

Q_SIGNALS:
    void playingChanged();
    void stateChanged();

public:
    void OnLibVlcEvent( const libvlc_event_t* e );

    //QML Api
    QString get_version();
    QString get_vlcVersion();

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void togglePause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void mute();
    Q_INVOKABLE void unMute();
    Q_INVOKABLE void toggleMute();

    bool get_playing();

    double get_length();

    double get_position();
    void set_position( double );

    double get_time();
    void set_time( double );

    unsigned int get_volume();
    void set_volume( unsigned int );

    bool get_fullscreen();
    void set_fullscreen( bool fs );

    int get_state();

    Q_INVOKABLE void toggleFullscreen();

Q_SIGNALS:
    /* async events from libvlc */
    void MediaPlayerMediaChanged();
    void MediaPlayerNothingSpecial();
    void MediaPlayerOpening();
    void MediaPlayerBuffering();
    void MediaPlayerPlaying();
    void MediaPlayerPaused();
    void MediaPlayerForward();
    void MediaPlayerBackward();
    void MediaPlayerEncounteredError();
    void MediaPlayerEndReached();
    void MediaPlayerStopped();

    void MediaPlayerTimeChanged();
    void MediaPlayerPositionChanged();
    void MediaPlayerSeekableChanged();
    void MediaPlayerPausableChanged();

public:
    QmlVlcAudio*     get_audio()     { return &m_audio; }
    QmlVlcInput*     get_input()     { return &m_input; }
    QmlVlcPlaylist*  get_playlist()  { return &m_playlist; }
    QmlVlcSubtitle*  get_subtitle()  { return &m_subtitle; }
    QmlVlcVideo*     get_video()     { return &m_video; }
    QmlVlcMediaDesc* get_mediaDesc() { return &m_mediaDesc; }

private:
    QmlVlcAudio     m_audio;
    QmlVlcInput     m_input;
    QmlVlcPlaylist  m_playlist;
    QmlVlcSubtitle  m_subtitle;
    QmlVlcVideo     m_video;
    QmlVlcMediaDesc m_mediaDesc;
};

#endif // QMLVLCPLAYER_H
