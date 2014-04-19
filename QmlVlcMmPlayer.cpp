#include "QmlVlcMmPlayer.h"

#include "QmlVlcConfig.h"

QmlVlcMmPlayer::QmlVlcMmPlayer( QObject* parent )
    : QmlVlcPlayerProxy( &m_player, parent ),
      m_libvlc( 0 ),
      m_videoOutput( &m_player )
{
    m_libvlc = QmlVlcConfig::createLibvlcInstance();
    if( m_libvlc ) {
        m_player.open( m_libvlc );
        m_videoOutput.init();
    }
}

QmlVlcMmPlayer::~QmlVlcMmPlayer()
{
    m_player.close();
    if( m_libvlc ) {
        libvlc_free( m_libvlc );
        m_libvlc = 0;
    }
}
