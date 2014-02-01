#include "QmlVlcDeinterlace.h"

void QmlVlcDeinterlace::enable( const QString& mode )
{
    libvlc_video_set_deinterlace( m_player.get_mp(), mode.toLatin1().constData() );
}

void QmlVlcDeinterlace::disable()
{
    libvlc_video_set_deinterlace( m_player.get_mp(), "" );
}
