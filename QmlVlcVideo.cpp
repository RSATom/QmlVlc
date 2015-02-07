/*******************************************************************************
* Copyright © 2014, Sergey Radionov <rsatom_gmail.com>
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

void QmlVlcVideo::toggleTeletext()
{
    libvlc_toggle_teletext( m_player.get_mp() );
}
