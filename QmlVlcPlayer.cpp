#include "QmlVlcPlayer.h"

#include "global/config.h"

QmlVlcPlayer::QmlVlcPlayer( vlc::player* player, QObject* parent /*= 0*/ )
    : QmlVlcVideoOutput( player, parent ),
      m_audio( *player ), m_input( *player ),
      m_playlist( *player ),  m_subtitle( *player ),
      m_video( *player ), m_mediaDesc( *player )
{
    connect( this, SIGNAL( MediaPlayerPlaying() ), this, SIGNAL( playingChanged() ) );
    connect( this, SIGNAL( MediaPlayerPaused() ), this, SIGNAL( playingChanged() ) );
    connect( this, SIGNAL( MediaPlayerEncounteredError() ), this, SIGNAL( playingChanged() ) );
    connect( this, SIGNAL( MediaPlayerEndReached() ), this, SIGNAL( playingChanged() ) );
    connect( this, SIGNAL( MediaPlayerStopped() ), this, SIGNAL( playingChanged() ) );

    connect( this, SIGNAL( MediaPlayerNothingSpecial() ), this, SIGNAL( stateChanged() ) );
    connect( this, SIGNAL( MediaPlayerOpening() ), this, SIGNAL( stateChanged() ) );
    connect( this, SIGNAL( MediaPlayerBuffering() ), this, SIGNAL( stateChanged() ) );
    connect( this, SIGNAL( MediaPlayerPlaying() ), this, SIGNAL( stateChanged() ) );
    connect( this, SIGNAL( MediaPlayerPaused() ), this, SIGNAL( stateChanged() ) );
    connect( this, SIGNAL( MediaPlayerEncounteredError() ), this, SIGNAL( stateChanged() ) );
    connect( this, SIGNAL( MediaPlayerEndReached() ), this, SIGNAL( stateChanged() ) );
    connect( this, SIGNAL( MediaPlayerStopped() ), this, SIGNAL( stateChanged() ) );
}

//libvlc events arrives from separate thread
void QmlVlcPlayer::OnLibVlcEvent( const libvlc_event_t* e )
{
    switch ( e->type ) {
    case libvlc_MediaPlayerMediaChanged:
        Q_EMIT MediaPlayerMediaChanged();
        break;
    case libvlc_MediaPlayerNothingSpecial:
        Q_EMIT MediaPlayerNothingSpecial();
        break;
    case libvlc_MediaPlayerOpening:
        Q_EMIT MediaPlayerOpening();
        break;
    case libvlc_MediaPlayerBuffering:
        Q_EMIT MediaPlayerBuffering();
        break;
    case libvlc_MediaPlayerPlaying:
        Q_EMIT MediaPlayerPlaying();
        break;
    case libvlc_MediaPlayerPaused:
        Q_EMIT MediaPlayerPaused();
        break;
    case libvlc_MediaPlayerStopped:
        Q_EMIT MediaPlayerStopped();
        break;
    case libvlc_MediaPlayerForward:
        Q_EMIT MediaPlayerForward();
        break;
    case libvlc_MediaPlayerBackward:
        Q_EMIT MediaPlayerBackward();
        break;
    case libvlc_MediaPlayerEndReached:
        Q_EMIT MediaPlayerEndReached();
        break;
    case libvlc_MediaPlayerEncounteredError:
        Q_EMIT MediaPlayerEncounteredError();
        break;
    case libvlc_MediaPlayerTimeChanged:
        Q_EMIT MediaPlayerTimeChanged();
        break;
    case libvlc_MediaPlayerPositionChanged:
        Q_EMIT MediaPlayerPositionChanged();
        break;
    case libvlc_MediaPlayerSeekableChanged:
        Q_EMIT MediaPlayerSeekableChanged();
        break;
    case libvlc_MediaPlayerPausableChanged:
        Q_EMIT MediaPlayerPausableChanged();
        break;
    //case libvlc_MediaPlayerTitleChanged:
    //    Q_EMIT MediaPlayerTitleChanged();
    //    break;
    //case libvlc_MediaPlayerSnapshotTaken:
    //    Q_EMIT MediaPlayerSnapshotTaken();
    //    break;
    //case libvlc_MediaPlayerLengthChanged:
    //    Q_EMIT MediaPlayerLengthChanged();
    //    break;
    //case libvlc_MediaPlayerVout:
    //    Q_EMIT MediaPlayerVout();
    //    break;
    };
}

QString QmlVlcPlayer::get_version()
{
    return QStringLiteral( FBSTRING_PLUGIN_VERSION );
}

QString QmlVlcPlayer::get_vlcVersion()
{
    return QString::fromLatin1( libvlc_get_version() );
}

void QmlVlcPlayer::play()
{
    player().play();
}

void QmlVlcPlayer::pause()
{
    player().pause();
}

void QmlVlcPlayer::togglePause()
{
    player().togglePause();
}

void QmlVlcPlayer::stop()
{
    //use async stop to avoid freeze (on network timeout for example).
    player().stop( true );
    player().clear_items();
}

void QmlVlcPlayer::mute()
{
    player().audio().set_mute( true );
}

void QmlVlcPlayer::unMute()
{
    player().audio().set_mute( false );
}

void QmlVlcPlayer::toggleMute()
{
    player().audio().toggle_mute();
}

bool QmlVlcPlayer::get_playing()
{
    return player().is_playing();
}

double QmlVlcPlayer::get_length()
{
    return static_cast<double>( player().current_media().get_length() );
}

double QmlVlcPlayer::get_position()
{
    return player().get_position();
}

void QmlVlcPlayer::set_position( double pos )
{
    player().set_position( static_cast<float>( pos ) );
}

double QmlVlcPlayer::get_time()
{
    return static_cast<double>( player().get_time() );
}

void QmlVlcPlayer::set_time( double t )
{
    player().set_time( static_cast<libvlc_time_t>( t ) );
}

unsigned int QmlVlcPlayer::get_volume()
{
    return player().audio().get_volume();
}

void QmlVlcPlayer::set_volume( unsigned int v )
{
    player().audio().set_volume( v );
}

int QmlVlcPlayer::get_state()
{
    return player().get_state();
};

bool QmlVlcPlayer::get_fullscreen()
{
    //FIXME!
    return false;
}

void QmlVlcPlayer::set_fullscreen( bool fs )
{
    //FIXME!
}

void QmlVlcPlayer::toggleFullscreen()
{
    //FIXME!
}
