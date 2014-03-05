#include "QmlVlcPlayer.h"

QmlVlcPlayerProxy::QmlVlcPlayerProxy( vlc::player* player, QObject* parent /*= 0*/ )
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
void QmlVlcPlayerProxy::OnLibVlcEvent( const libvlc_event_t* e )
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

QString QmlVlcPlayerProxy::get_vlcVersion()
{
    return QString::fromLatin1( libvlc_get_version() );
}

void QmlVlcPlayerProxy::play( const QString& mrl )
{
    vlc_player& p = player();

    p.clear_items();

    int item = p.add_media( mrl.toUtf8().data(), 0, 0, 0, 0 );
    if( item >= 0) {
        p.play( item );
    }
}

void QmlVlcPlayerProxy::play()
{
    player().play();
}

void QmlVlcPlayerProxy::pause()
{
    player().pause();
}

void QmlVlcPlayerProxy::togglePause()
{
    player().togglePause();
}

void QmlVlcPlayerProxy::stop()
{
    //use async stop to avoid freeze (on network timeout for example).
    player().stop( true );
    player().clear_items();
}

void QmlVlcPlayerProxy::mute()
{
    player().audio().set_mute( true );
}

void QmlVlcPlayerProxy::unMute()
{
    player().audio().set_mute( false );
}

void QmlVlcPlayerProxy::toggleMute()
{
    player().audio().toggle_mute();
}

QString QmlVlcPlayerProxy::get_mrl()
{
    return QString::fromUtf8( player().current_media().get_mrl() );
}

void QmlVlcPlayerProxy::set_mrl( const QString& mrl )
{
    play( mrl );
}

bool QmlVlcPlayerProxy::get_playing()
{
    return player().is_playing();
}

double QmlVlcPlayerProxy::get_length()
{
    return static_cast<double>( player().current_media().get_length() );
}

double QmlVlcPlayerProxy::get_position()
{
    return player().get_position();
}

void QmlVlcPlayerProxy::set_position( double pos )
{
    player().set_position( static_cast<float>( pos ) );
}

double QmlVlcPlayerProxy::get_time()
{
    return static_cast<double>( player().get_time() );
}

void QmlVlcPlayerProxy::set_time( double t )
{
    player().set_time( static_cast<libvlc_time_t>( t ) );
}

unsigned int QmlVlcPlayerProxy::get_volume()
{
    return player().audio().get_volume();
}

void QmlVlcPlayerProxy::set_volume( unsigned int v )
{
    player().audio().set_volume( v );
}

int QmlVlcPlayerProxy::get_state()
{
    return player().get_state();
}

bool QmlVlcPlayerProxy::get_fullscreen()
{
    //FIXME!
    return false;
}

void QmlVlcPlayerProxy::set_fullscreen( bool /*fs*/ )
{
    //FIXME!
}

void QmlVlcPlayerProxy::toggleFullscreen()
{
    //FIXME!
}

QmlVlcPlayer::QmlVlcPlayer( QObject* parent )
    : QmlVlcPlayerProxy( &m_player, parent )
{
    m_libvlc = libvlc_new( 0, 0 );
    m_player.open( m_libvlc );
}

QmlVlcPlayer::~QmlVlcPlayer()
{
    vlcEvents( false );

    m_player.close();
    if( m_libvlc ) {
        libvlc_free( m_libvlc );
        m_libvlc = 0;
    }
}

void QmlVlcPlayer::classBegin()
{
    QmlVlcPlayerProxy::classBegin();

    vlcEvents( true );
}

void QmlVlcPlayer::vlcEvents( bool Attach )
{
    if( !player().is_open() )
        return;

    libvlc_event_manager_t* em =
        libvlc_media_player_event_manager( player().get_mp() );
    if( !em )
        return;

    for( int e = libvlc_MediaPlayerMediaChanged; e <= libvlc_MediaPlayerVout; ++e ) {
        switch( e ){
        case libvlc_MediaPlayerMediaChanged:
        case libvlc_MediaPlayerNothingSpecial:
        case libvlc_MediaPlayerOpening:
        case libvlc_MediaPlayerBuffering:
        case libvlc_MediaPlayerPlaying:
        case libvlc_MediaPlayerPaused:
        case libvlc_MediaPlayerStopped:
        case libvlc_MediaPlayerForward:
        case libvlc_MediaPlayerBackward:
        case libvlc_MediaPlayerEndReached:
        case libvlc_MediaPlayerEncounteredError:
        case libvlc_MediaPlayerTimeChanged:
        case libvlc_MediaPlayerPositionChanged:
        case libvlc_MediaPlayerSeekableChanged:
        case libvlc_MediaPlayerPausableChanged:
        //case libvlc_MediaPlayerTitleChanged:
        //case libvlc_MediaPlayerSnapshotTaken:
        //case libvlc_MediaPlayerLengthChanged:
        //case libvlc_MediaPlayerVout:
            if( Attach )
                libvlc_event_attach( em, e, OnLibVlcEvent_proxy, this );
            else
                libvlc_event_detach( em, e, OnLibVlcEvent_proxy, this );
            break;
        }
    }
}

//libvlc events arrives from separate thread
void QmlVlcPlayer::OnLibVlcEvent_proxy( const libvlc_event_t* e, void *param )
{
    QmlVlcPlayerProxy* this_ = static_cast<QmlVlcPlayerProxy*>( param );
    this_->OnLibVlcEvent( e );
}
