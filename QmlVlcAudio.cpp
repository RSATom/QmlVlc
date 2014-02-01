#include "QmlVlcAudio.h"

unsigned QmlVlcAudio::get_trackCount()
{
    return m_player.audio().track_count();
}

bool QmlVlcAudio::get_mute()
{
    return m_player.audio().is_muted();
}

void QmlVlcAudio::set_mute( bool m )
{
    m_player.audio().set_mute( m );
}

unsigned int QmlVlcAudio::get_volume()
{
    return m_player.audio().get_volume();
}

void QmlVlcAudio::set_volume( unsigned int vol )
{
    m_player.audio().set_volume( vol );
}

int QmlVlcAudio::get_track()
{
    return m_player.audio().get_track();
}

void QmlVlcAudio::set_track( int idx )
{
    if( idx < 0 )
        return;

    m_player.audio().set_track( idx );
}

unsigned int QmlVlcAudio::get_channel()
{
    return m_player.audio().get_channel();
}

void QmlVlcAudio::set_channel( unsigned int ch )
{
    m_player.audio().set_channel( (libvlc_audio_output_channel_t) ch );
}

void QmlVlcAudio::toggleMute()
{
    m_player.audio().toggle_mute();
}

QString QmlVlcAudio::description( unsigned int trackID )
{
    QString track_name;

    libvlc_track_description_t* root_track_desc =
        libvlc_audio_get_track_description( m_player.get_mp() );
    if( !root_track_desc )
        return track_name;

    unsigned int tc = m_player.audio().track_count();
    if( tc && trackID < tc ) {
        libvlc_track_description_t* track_desc = root_track_desc;
        for( ; trackID && track_desc ; --trackID ){
            track_desc = track_desc->p_next;
        }

        if ( track_desc && track_desc->psz_name ) {
            track_name = track_desc->psz_name;
        }
    }
    libvlc_track_description_list_release( root_track_desc );

    return track_name;
}
