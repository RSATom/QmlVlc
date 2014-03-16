#include "QmlVlcPlayer.h"

QmlVlcPlayer::QmlVlcPlayer( QObject* parent )
    : QmlVlcSurfacePlayerProxy( &m_player, parent )
{
    m_libvlc = libvlc_new( 0, 0 );
    m_player.open( m_libvlc );
}

QmlVlcPlayer::~QmlVlcPlayer()
{
    m_player.close();
    if( m_libvlc ) {
        libvlc_free( m_libvlc );
        m_libvlc = 0;
    }
}
