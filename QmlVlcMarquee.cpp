#include "QmlVlcMarquee.h"

#include "QmlVlcPositions.h"

QString QmlVlcMarquee::get_text()
{
    QString text;
    char* t = libvlc_video_get_marquee_string( m_player.get_mp(), libvlc_marquee_Text );
    if ( t )
        text = t;

    return text;
}

void QmlVlcMarquee::set_text( const QString& t )
{
    libvlc_video_set_marquee_string( m_player.get_mp(), libvlc_marquee_Text, t.toUtf8().constData() );
}

QString QmlVlcMarquee::get_position()
{
    int p = get_marquee_int( libvlc_marquee_Position );
    return QmlGetPositionById( p );
}

void QmlVlcMarquee::set_position( const QString& position )
{
    int p = QmlGetIdByPosition( position );
    set_marquee_int( libvlc_marquee_Position, p );
}

int QmlVlcMarquee::get_marquee_int( libvlc_video_marquee_option_t o )
{
    return libvlc_video_get_marquee_int( m_player.get_mp(), o );
}

void QmlVlcMarquee::set_marquee_int( libvlc_video_marquee_option_t o, int i )
{
    libvlc_video_set_marquee_int( m_player.get_mp(), o, i );
}
