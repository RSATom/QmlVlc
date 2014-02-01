#include "QmlVlcSubtitle.h"

unsigned QmlVlcSubtitle::get_trackCount()
{
    return m_player.subtitles().track_count();
}

int QmlVlcSubtitle::get_track()
{
    return m_player.subtitles().get_track();
}

void QmlVlcSubtitle::set_track( int idx )
{
    if( idx < 0 )
        return;

    return m_player.subtitles().set_track( idx );
}

QString QmlVlcSubtitle::description( unsigned int sID )
{
    QString s_name;

    libvlc_track_description_t* root_s_desc =
        libvlc_video_get_spu_description( m_player.get_mp() );
    if( !root_s_desc )
        return s_name;

    unsigned int sc = libvlc_video_get_spu_count( m_player.get_mp() );
    if( sc && sID < sc ) {
        libvlc_track_description_t* s_desc = root_s_desc;
        for( ; sID && s_desc ; --sID ){
            s_desc = s_desc->p_next;
        }

        if ( s_desc && s_desc->psz_name ) {
            s_name = s_desc->psz_name;
        }
    }
    libvlc_track_description_list_release( root_s_desc );

    return s_name;
}
