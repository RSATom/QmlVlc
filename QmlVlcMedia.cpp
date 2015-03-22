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

#include "QmlVlcMedia.h"

#include "QmlVlcPlayerProxy.h"

////////////////////////////////////////////////////////////////////////////
/// QmlVlcMedia
////////////////////////////////////////////////////////////////////////////
QmlVlcMedia::QmlVlcMedia( QmlVlcPlayerProxy* mediaOwner )
    : m_mediaOwner( mediaOwner )
{
}

QString QmlVlcMedia::get_meta( libvlc_meta_t e_meta )
{
    std::string meta = get_media().meta( e_meta );
    return QString::fromUtf8( meta.data(), meta.size() );
}

void QmlVlcMedia::set_meta( libvlc_meta_t e_meta, const QString& meta )
{
    get_media().set_meta( e_meta, meta.toStdString() );

    if( libvlc_meta_Title == e_meta )
        Q_EMIT titleChanged();
}

QString QmlVlcMedia::get_title()
{
    return get_meta( libvlc_meta_Title );
}

void QmlVlcMedia::set_title( const QString& title )
{
    set_meta( libvlc_meta_Title, title );
}

QString QmlVlcMedia::get_artist()
{
    return get_meta( libvlc_meta_Artist );
}

QString QmlVlcMedia::get_genre()
{
    return get_meta( libvlc_meta_Genre );
}

QString QmlVlcMedia::get_copyright()
{
    return get_meta( libvlc_meta_Copyright );
}

QString QmlVlcMedia::get_album()
{
    return get_meta( libvlc_meta_Album );
}

QString QmlVlcMedia::get_trackNumber()
{
    return get_meta( libvlc_meta_TrackNumber );
}

QString QmlVlcMedia::get_description()
{
    return get_meta( libvlc_meta_Description );
}

QString QmlVlcMedia::get_rating()
{
    return get_meta( libvlc_meta_Rating );
}

QString QmlVlcMedia::get_date()
{
    return get_meta( libvlc_meta_Date );
}

QString QmlVlcMedia::get_setting()
{
    return get_meta( libvlc_meta_Setting );
}

void QmlVlcMedia::set_setting( const QString& setting )
{
    set_meta( libvlc_meta_Setting, setting );
}

QString QmlVlcMedia::get_URL()
{
    return get_meta( libvlc_meta_URL );
}

QString QmlVlcMedia::get_language()
{
    return get_meta( libvlc_meta_Language );
}

QString QmlVlcMedia::get_nowPlaying()
{
    return get_meta( libvlc_meta_NowPlaying );
}

QString QmlVlcMedia::get_publisher()
{
    return get_meta( libvlc_meta_Publisher );
}

QString QmlVlcMedia::get_encodedBy()
{
    return get_meta( libvlc_meta_EncodedBy );
}

QString QmlVlcMedia::get_artworkURL()
{
    return get_meta( libvlc_meta_ArtworkURL );
}

QString QmlVlcMedia::get_trackID()
{
    return get_meta( libvlc_meta_TrackID );
}

QString QmlVlcMedia::get_mrl()
{
    std::string mrl = get_media().mrl();
    return QString::fromUtf8( mrl.data(), mrl.size() );
}

bool QmlVlcMedia::get_disabled()
{
    if( !m_mediaOwner )
        return false;

    vlc::player& player = m_mediaOwner->player();

    int idx = player.find_media_index( get_media() );
    return idx < 0 ? false : player.is_item_disabled( idx );
}

void QmlVlcMedia::set_disabled( bool disabled )
{
    if( !m_mediaOwner )
        return;

    vlc::player& player = m_mediaOwner->player();

    int idx = player.find_media_index( get_media() );
    if( idx >= 0 ) {
        player.disable_item( idx, disabled );
    }
}

////////////////////////////////////////////////////////////////////////////
/// QmlVlcCurrentMedia
////////////////////////////////////////////////////////////////////////////
QmlVlcCurrentMedia::QmlVlcCurrentMedia( QmlVlcPlayerProxy* mediaOwner )
    : QmlVlcMedia( mediaOwner )
{
}

vlc::media QmlVlcCurrentMedia::get_media() const
{
    if( !m_mediaOwner )
        return vlc::media();

    return m_mediaOwner->player().current_media();
}

////////////////////////////////////////////////////////////////////////////
/// QmlVlcMediaMedia
////////////////////////////////////////////////////////////////////////////
QmlVlcMediaMedia::QmlVlcMediaMedia( QmlVlcPlayerProxy* mediaOwner,
                                    const vlc::media& media )
    : QmlVlcMedia( mediaOwner ), m_media( media )
{
}

vlc::media QmlVlcMediaMedia::get_media() const
{
    if( !m_mediaOwner )
        return vlc::media();

    return m_media;
}
