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

#include "QmlVlcSubtitle.h"

#include <QDir>
#include <QUrl>
#include <QNetworkRequest>

QmlVlcSubtitle::QmlVlcSubtitle( vlc::player& player )
    : m_player( player ), m_networkReply( nullptr )
{
}

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

int QmlVlcSubtitle::get_delay()
{
    return m_player.subtitles().get_delay();
}

void QmlVlcSubtitle::set_delay( int delay )
{
    m_player.subtitles().set_delay( delay );
}

void QmlVlcSubtitle::networkDataReady()
{
    m_downloadingFile.write( m_networkReply->readAll() );
}

void QmlVlcSubtitle::downloadFinished()
{
    if( QNetworkReply::NoError == m_networkReply->error() ) {
        m_downloadingFile.rename( m_downloadingFile.fileName() + m_networkReply->url().fileName() );
        const QString nativePath =
            QDir::toNativeSeparators( m_downloadingFile.fileName() );
        if( m_player.subtitles().load( nativePath.toUtf8().constData() ) ) {
            Q_EMIT loadFinished();
        } else {
            Q_EMIT loadError();
        }
    } else {
        Q_EMIT loadError();
    }
    m_downloadingFile.close();
    m_networkReply->deleteLater();
    m_networkReply = nullptr;
}

void QmlVlcSubtitle::load( const QUrl& url )
{
    if( m_networkReply )
        return;

    if( url.isLocalFile() ) {
        const QString nativePath = QDir::toNativeSeparators( url.toLocalFile() );
        if( m_player.subtitles().load( nativePath.toUtf8().constData() ) ) {
            Q_EMIT loadFinished();
        } else {
            Q_EMIT loadError();
        }
    } else {
        m_downloadingFile.setFileTemplate( m_downloadingFile.fileTemplate() );
        if( !m_downloadingFile.open() ) {
            Q_EMIT loadError();
        }

        m_networkReply = m_networkManager.get( QNetworkRequest( url ) );
        m_networkReply->setReadBufferSize( 4096 );
        connect( m_networkReply, &QNetworkReply::readyRead,
                 this, &QmlVlcSubtitle::networkDataReady );
        connect( m_networkReply, &QNetworkReply::finished,
                 this, &QmlVlcSubtitle::downloadFinished );
    }
}
