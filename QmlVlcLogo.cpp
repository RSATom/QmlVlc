#include "QmlVlcLogo.h"

#include "QmlVlcPositions.h"

QString QmlVlcLogo::get_position()
{
    int p = get_logo_int( libvlc_logo_position );
    return QmlGetPositionById( p );
}

void QmlVlcLogo::set_position( const QString& position )
{
    int p = QmlGetIdByPosition( position );
    set_logo_int( libvlc_logo_position, p );
}

void QmlVlcLogo::file( const QString& f )
{
    libvlc_video_set_logo_string( m_player.get_mp(), libvlc_logo_file, f.toUtf8().constData() );
}

int QmlVlcLogo::get_logo_int( libvlc_video_logo_option_t o )
{
    return libvlc_video_get_logo_int( m_player.get_mp(), o );
}

void QmlVlcLogo::set_logo_int( libvlc_video_logo_option_t o, int i )
{
    libvlc_video_set_logo_int( m_player.get_mp(), o, i );
}
