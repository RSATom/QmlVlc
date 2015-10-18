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

#include "QmlVlcPlayerProxy.h"

#include <QDebug>
#include <QCoreApplication>

QmlVlcPlayerProxy::LibvlcEvent::LibvlcEvent( const libvlc_event_t& libvlcEvent )
    : QEvent( static_cast<QEvent::Type>( LibvlcEventId ) ),
      _libvlcEvent( libvlcEvent )
{
}

QmlVlcPlayerProxy::QmlVlcPlayerProxy( const std::shared_ptr<vlc::playlist_player_core>& player,
                                      QObject* parent /*= 0*/ )
    : QmlVlcVideoSource( player, parent ), m_player( player ), m_audio( *player ), m_input( *player ),
      m_playlist( this ), m_subtitle( *player ), m_video( *player ),
      m_currentMediaDesc( this )
{
    qRegisterMetaType<QmlVlcPlayerProxy::State>( "QmlVlcPlayerProxy::State" );

    m_errorTimer.setInterval( 1000 );
    m_errorTimer.setSingleShot( true );

    connect( this, SIGNAL( mediaPlayerEncounteredError() ),
             &m_errorTimer, SLOT( start() ) );
    connect( &m_errorTimer, SIGNAL( timeout() ),
             this, SLOT( currentItemEndReached() ) );

    connect( this, SIGNAL( mediaPlayerEndReached() ),
             &m_errorTimer, SLOT( stop() ) );
    connect( this, SIGNAL( mediaPlayerEndReached() ),
             this, SLOT( currentItemEndReached() ) );

    connect( this, SIGNAL( mediaPlayerMediaChanged() ),
             &m_errorTimer, SLOT( stop() ) );
    connect( this, &QmlVlcPlayerProxy::mediaPlayerMediaChanged,
             get_subtitle(), &QmlVlcSubtitle::eraseLoaded );

    connect( get_audio(), SIGNAL( volumeChanged() ),
             this, SIGNAL( volumeChanged() ) );
}

void QmlVlcPlayerProxy::classBegin()
{
    player().register_callback( this );
    QmlVlcVideoSource::classBegin();
}

void QmlVlcPlayerProxy::componentComplete()
{
}

void QmlVlcPlayerProxy::classEnd()
{
    if( m_player ) {
        player().unregister_callback( this );
    }
}

QmlVlcPlayerProxy::~QmlVlcPlayerProxy()
{
    classEnd();
}

static void LogEvent( const libvlc_event_t* e )
{
    switch ( e->type ) {
    case libvlc_MediaPlayerMediaChanged:
        qDebug() << "mediaPlayerMediaChanged";
        break;
    case libvlc_MediaPlayerNothingSpecial:
        qDebug() << "mediaPlayerNothingSpecial";
        break;
    case libvlc_MediaPlayerOpening:
        qDebug() << "mediaPlayerOpening";
        break;
    case libvlc_MediaPlayerBuffering:
        qDebug() << "mediaPlayerBuffering";
        break;
    case libvlc_MediaPlayerPlaying:
        qDebug() << "mediaPlayerPlaying";
        break;
    case libvlc_MediaPlayerPaused:
        qDebug() << "mediaPlayerPaused";
        break;
    case libvlc_MediaPlayerStopped:
        qDebug() << "mediaPlayerStopped";
        break;
    case libvlc_MediaPlayerForward:
        qDebug() << "mediaPlayerForward";
        break;
    case libvlc_MediaPlayerBackward:
        qDebug() << "mediaPlayerBackward";
        break;
    case libvlc_MediaPlayerEndReached:
        qDebug() << "mediaPlayerEndReached";
        break;
    case libvlc_MediaPlayerEncounteredError:
        qDebug() << "mediaPlayerEncounteredError";
        break;
    case libvlc_MediaPlayerTimeChanged:
        qDebug() << "mediaPlayerTimeChanged";
        break;
    case libvlc_MediaPlayerPositionChanged:
        qDebug() << "mediaPlayerPositionChanged";
        break;
    case libvlc_MediaPlayerSeekableChanged:
        qDebug() << "mediaPlayerSeekableChanged";
        break;
    case libvlc_MediaPlayerPausableChanged:
        qDebug() << "mediaPlayerPausableChanged";
        break;
    case libvlc_MediaPlayerTitleChanged:
        qDebug() << "mediaPlayerTitleChanged";
        break;
    case libvlc_MediaPlayerLengthChanged:
        qDebug() << "mediaPlayerLengthChanged";
        break;
    };
}

//libvlc events could arrive from separate thread
void QmlVlcPlayerProxy::media_player_event( const libvlc_event_t* e )
{
    //to avoid deadlocks have to always queue libvlc events,
    //since libvlc is not reentrant
    QCoreApplication::postEvent( this, new LibvlcEvent( *e ) );
}

bool QmlVlcPlayerProxy::event( QEvent* event )
{
    if( static_cast<unsigned>( event->type() ) == LibvlcEvent::LibvlcEventId ) {
        LibvlcEvent* libvlcEvent = static_cast<LibvlcEvent*>( event );
        handleLibvlcEvent( *libvlcEvent );
        return true;
    }

    return QObject::event( event );
}

void QmlVlcPlayerProxy::handleLibvlcEvent( const LibvlcEvent& event )
{
    //it's highly possible libvlc_event_t will have wrong pointers inside at this point
    const libvlc_event_t* e = &event._libvlcEvent;

    //LogEvent( e );

    switch ( e->type ) {
    case libvlc_MediaPlayerMediaChanged:
        Q_EMIT mediaPlayerMediaChanged();
        Q_EMIT m_playlist.currentItemChanged();
        break;
    case libvlc_MediaPlayerNothingSpecial:
        Q_EMIT mediaPlayerNothingSpecial();
        Q_EMIT stateChanged( NothingSpecial );
        break;
    case libvlc_MediaPlayerOpening:
        Q_EMIT mediaPlayerOpening();
        Q_EMIT stateChanged( Opening );
        break;
    case libvlc_MediaPlayerBuffering:
        Q_EMIT mediaPlayerBuffering( e->u.media_player_buffering.new_cache );
        Q_EMIT stateChanged( Buffering );
        break;
    case libvlc_MediaPlayerPlaying:
        Q_EMIT mediaPlayerPlaying();
        Q_EMIT stateChanged( Playing );
        Q_EMIT playingChanged();
        break;
    case libvlc_MediaPlayerPaused:
        Q_EMIT mediaPlayerPaused();
        Q_EMIT stateChanged( Paused );
        Q_EMIT playingChanged();
        break;
    case libvlc_MediaPlayerStopped:
        Q_EMIT mediaPlayerStopped();
        Q_EMIT stateChanged( Stopped );
        Q_EMIT playingChanged();
        break;
    case libvlc_MediaPlayerForward:
        Q_EMIT mediaPlayerForward();
        break;
    case libvlc_MediaPlayerBackward:
        Q_EMIT mediaPlayerBackward();
        break;
    case libvlc_MediaPlayerEndReached:
        Q_EMIT mediaPlayerEndReached();
        Q_EMIT stateChanged( Ended );
        Q_EMIT playingChanged();
        break;
    case libvlc_MediaPlayerEncounteredError:
        Q_EMIT mediaPlayerEncounteredError();
        Q_EMIT stateChanged( Error );
        Q_EMIT playingChanged();
        break;
    case libvlc_MediaPlayerTimeChanged: {
        double new_time = static_cast<double>( e->u.media_player_time_changed.new_time );
        Q_EMIT mediaPlayerTimeChanged( new_time );
        break;
    }
    case libvlc_MediaPlayerPositionChanged:
        Q_EMIT mediaPlayerPositionChanged( e->u.media_player_position_changed.new_position );
        break;
    case libvlc_MediaPlayerSeekableChanged:
        Q_EMIT mediaPlayerSeekableChanged( e->u.media_player_seekable_changed.new_seekable != 0 );
        break;
    case libvlc_MediaPlayerPausableChanged:
        Q_EMIT mediaPlayerPausableChanged( e->u.media_player_pausable_changed.new_pausable != 0 );
        break;
    case libvlc_MediaPlayerTitleChanged:
        Q_EMIT mediaPlayerTitleChanged();
        Q_EMIT m_currentMediaDesc.titleChanged();
        break;
    case libvlc_MediaPlayerLengthChanged: {
            double new_length =
                static_cast<double>( e->u.media_player_length_changed.new_length );
            Q_EMIT mediaPlayerLengthChanged( new_length );
        }
        break;
    };
}

void QmlVlcPlayerProxy::currentItemEndReached()
{
    if( vlc::mode_single != player().get_playback_mode() )
        player().next();
}

QString QmlVlcPlayerProxy::get_vlcVersion()
{
    return QString::fromLatin1( libvlc_get_version() );
}

void QmlVlcPlayerProxy::play( const QString& mrl )
{
    vlc::playlist_player_core& p = player();

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
    player().stop();
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
    std::string mrl = player().current_media().mrl();
    return QString::fromUtf8( mrl.data(), mrl.size() );
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
    return static_cast<double>( player().playback().get_length() );
}

double QmlVlcPlayerProxy::get_position()
{
    return player().playback().get_position();
}

void QmlVlcPlayerProxy::set_position( double pos )
{
    player().playback().set_position( static_cast<float>( pos ) );
}

double QmlVlcPlayerProxy::get_time()
{
    return static_cast<double>( player().playback().get_time() );
}

void QmlVlcPlayerProxy::set_time( double t )
{
    player().playback().set_time( static_cast<libvlc_time_t>( t ) );
}

unsigned int QmlVlcPlayerProxy::get_volume()
{
    return player().audio().get_volume();
}

void QmlVlcPlayerProxy::set_volume( unsigned int v )
{
    player().audio().set_volume( v );
}

QmlVlcPlayerProxy::State QmlVlcPlayerProxy::get_state()
{
    return static_cast<State>( player().get_state() );
}
