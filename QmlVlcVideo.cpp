#include "QmlVlcVideo.h"

void QmlVlcVideo::getVideoSize( unsigned* width, unsigned* height )
{
    libvlc_media_t* media = libvlc_media_player_get_media( m_player.get_mp() );
    if( media && !libvlc_media_is_parsed( media ) )
        libvlc_media_parse( media );

    *width = *height = 0;
    libvlc_video_get_size( m_player.get_mp(), 0, width, height );

    if( media && ( !*width || !*height ) ) {
        /*FIXME: It's not absolutely correct way to detect media dimensions,
        since now will be returned dimensions of first track with not zero demensions,
        and there are no any guarantee it will be be current playing track.
        But we nothing can do with it, since there are no way to match current
        playing track and track info received from libvlc_media_get_tracks_info for now.*/
        libvlc_media_track_info_t* info;
        int infoCount = libvlc_media_get_tracks_info( media, &info );
        for( int i = 0; i < infoCount; ++i ) {
            if( libvlc_track_video == info[i].i_type &&
                info[i].u.video.i_width &&
                info[i].u.video.i_height )
            {
                *width = info[i].u.video.i_width;
                *height = info[i].u.video.i_height;
                break;
            }
        }
        libvlc_free( info );
    }
}

unsigned int QmlVlcVideo::get_width()
{
    unsigned x = 0, y = 0;
    getVideoSize( &x, &y );

    return x;
}

unsigned int QmlVlcVideo::get_height()
{
    unsigned x = 0, y = 0;
    getVideoSize( &x, &y );

    return y;
}

unsigned QmlVlcVideo::get_trackCount()
{
    return m_player.video().track_count();
}

int QmlVlcVideo::get_track()
{
    return m_player.video().get_track();
}

void QmlVlcVideo::set_track( int idx )
{
    if( idx < 0 )
        return;

    m_player.video().set_track( idx );
}

bool QmlVlcVideo::get_fullscreen()
{
    //FIXME!
    return false;
}

void QmlVlcVideo::set_fullscreen( bool fs )
{
    //FIXME!
}

void QmlVlcVideo::toggleFullscreen()
{
    //FIXME!
}

QString QmlVlcVideo::get_aspectRatio()
{
    QString aspectRatio;
    char* ar = libvlc_video_get_aspect_ratio( m_player.get_mp() );
    if ( ar )
        aspectRatio = ar;
    libvlc_free( ar );

    return aspectRatio;
}

void QmlVlcVideo::set_aspectRatio( const QString& ar )
{
    libvlc_video_set_aspect_ratio( m_player.get_mp(), ar.toLatin1().constData() );
}

int QmlVlcVideo::get_subtitle()
{
    return libvlc_video_get_spu( m_player.get_mp() );
}

void QmlVlcVideo::set_subtitle( unsigned int t )
{
    libvlc_video_set_spu( m_player.get_mp(), t );
}

QString QmlVlcVideo::get_crop()
{
    QString crop;
    char* c = libvlc_video_get_crop_geometry( m_player.get_mp() );
    if ( c )
        crop = c;
    libvlc_free( c );

    return crop;
}

void QmlVlcVideo::set_crop( const QString& c )
{
    libvlc_video_set_crop_geometry( m_player.get_mp(), c.toLatin1().constData() );
}

int QmlVlcVideo::get_teletext()
{
    return libvlc_video_get_teletext( m_player.get_mp() );
}

void QmlVlcVideo::set_teletext( unsigned int t )
{
    return libvlc_video_set_teletext( m_player.get_mp(), t );
}

void QmlVlcVideo::toggleTeletext()
{
    libvlc_toggle_teletext( m_player.get_mp() );
}

float QmlVlcVideo::get_contrast()
{
    return m_player.video().get_contrast();
}

void QmlVlcVideo::set_contrast( float v )
{
    return m_player.video().set_contrast( v );
}

float QmlVlcVideo::get_brightness()
{
    return m_player.video().get_brightness();
}

void QmlVlcVideo::set_brightness( float v )
{
    return m_player.video().set_brightness( v );
}

float QmlVlcVideo::get_hue()
{
    return m_player.video().get_hue();
}

void QmlVlcVideo::set_hue( float v )
{
    return m_player.video().set_hue( v );
}

float QmlVlcVideo::get_saturation()
{
    return m_player.video().get_saturation();
}

void QmlVlcVideo::set_saturation( float v )
{
    return m_player.video().set_saturation( v );
}

float QmlVlcVideo::get_gamma()
{
    return m_player.video().get_gamma();
}

void QmlVlcVideo::set_gamma( float v )
{
    return m_player.video().set_gamma( v );
}
