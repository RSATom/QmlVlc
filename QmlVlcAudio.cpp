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

QmlVlcAudio::Output QmlVlcAudio::get_channel()
{
    return static_cast<Output>( m_player.audio().get_channel() );
}

void QmlVlcAudio::set_channel( QmlVlcAudio::Output ch )
{
    m_player.audio().set_channel( static_cast<libvlc_audio_output_channel_t>( ch ) );
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

int QmlVlcAudio::get_delay()
{
    return m_player.audio().get_delay();
}

void QmlVlcAudio::set_delay( int delay )
{
    m_player.audio().set_delay( delay );
}
