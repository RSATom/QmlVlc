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

#include <cassert>

#include <QDir>
#include <QUrl>
#include <QNetworkRequest>

enum {
    MAX_SUBTITLE_SIZE    = ( 1 << 20 ), //= 1MB
    MAX_LOADED_SUBTITLES = 30,
};

QmlVlcSubtitle::QmlVlcSubtitle( vlc::player& player )
    : m_player( player ), m_networkReply( nullptr ), m_loadedCount( 0 )
{
 QT_BEGIN_NAMESPACE}

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

bool QmlVlcSubtitle::loadFromFile( QTemporaryFile* subtitleFile )
{
    const QString nativePath =
        QDir::toNativeSeparators( subtitleFile->fileName() );
    if( m_player.subtitles().load( nativePath.toUtf8().constData() ) ) {
        subtitleFile->setAutoRemove( false );
        ++m_loadedCount;
        return true;
    }

    return false;
}

void QmlVlcSubtitle::networkDataReady()
{
    if( !m_subtitleFile || !m_networkReply ) {
        assert( false );
        return;
    }

    if( m_subtitleFile->size() < MAX_SUBTITLE_SIZE )
        m_subtitleFile->write( m_networkReply->readAll() );
    else
        m_networkReply->abort();
}

void QmlVlcSubtitle::downloadFinished()
{
    if( !m_networkReply ) {
        assert( false );
        return;
    }

    if( QNetworkReply::NoError == m_networkReply->error() && m_subtitleFile &&
        m_subtitleFile->rename( m_subtitleFile->fileName() + QStringLiteral( "." ) +
                                QFileInfo( m_networkReply->url().fileName() ).suffix() ) &&
        loadFromFile( m_subtitleFile.data() ) )
    {
        Q_EMIT loadFinished();
    } else {
        Q_EMIT loadError();
    }

    m_networkReply->deleteLater();
    m_networkReply = nullptr;
    m_subtitleFile.reset();
}

void QmlVlcSubtitle::load( const QUrl& url )
{
    if( m_networkReply  )
        return;

    assert( !m_subtitleFile );

    if( m_loadedCount >= MAX_LOADED_SUBTITLES ) {
        Q_EMIT loadError();
        return;
    }

    if( url.isLocalFile() ) {
        const QString nativePath = QDir::toNativeSeparators( url.toLocalFile() );
        if( m_player.subtitles().load( nativePath.toUtf8().constData() ) ) {
            Q_EMIT loadFinished();
        } else {
            Q_EMIT loadError();
        }
    } else {
        if( !m_subtitlesDir )
            m_subtitlesDir.reset( new QTemporaryDir );

        if( !m_subtitlesDir->isValid() ) {
            Q_EMIT loadError();
            return;
        }

        QScopedPointer<QTemporaryFile> subtitleFile(
            new QTemporaryFile( m_subtitlesDir->path() +
                                QDir::separator() +
                                QStringLiteral( "url-sub-XXXXXX" ) ) );
        if( !subtitleFile->open() ) {
            Q_EMIT loadError();
            return;
        }

        m_subtitleFile.swap( subtitleFile );

        m_networkReply = m_networkManager.get( QNetworkRequest( url ) );
        m_networkReply->setReadBufferSize( 4096 );
        connect( m_networkReply, &QNetworkReply::readyRead,
                 this, &QmlVlcSubtitle::networkDataReady );
        connect( m_networkReply, &QNetworkReply::finished,
                 this, &QmlVlcSubtitle::downloadFinished );
    }
}

QString GetExtension( QmlVlcSubtitle::Type type )
{
    switch( type ) {
        case QmlVlcSubtitle::Microdvd:
            return QStringLiteral( "sub" );
        case QmlVlcSubtitle::Subrip:
            return QStringLiteral( "srt" );
        //case QmlVlcSubtitle::Subviewer:
        //case QmlVlcSubtitle::Ssa1:
        //case QmlVlcSubtitle::Ssa2_4:
        //case QmlVlcSubtitle::Ass:
        //case QmlVlcSubtitle::Vplayer:
        //case QmlVlcSubtitle::Sami:
        //case QmlVlcSubtitle::Dvdsubtitle:
        //case QmlVlcSubtitle::Mpl2:
        //case QmlVlcSubtitle::Aqt:
        //case QmlVlcSubtitle::Pjs:
        //case QmlVlcSubtitle::Mpsub:
        //case QmlVlcSubtitle::Jacosub:
        //case QmlVlcSubtitle::Psb:
        //case QmlVlcSubtitle::Realtext:
        case QmlVlcSubtitle::Dks:
            return QStringLiteral( "dks" );
        //case QmlVlcSubtitle::Subviewer1:
        case QmlVlcSubtitle::Vtt:
            return QStringLiteral( "vtt" );
    }

    assert( false );
    return QString();
}

bool QmlVlcSubtitle::loadFromString( const QByteArray& subtitle, Type type )
{
    const QString ext = GetExtension( type );
    if( m_loadedCount >= MAX_LOADED_SUBTITLES ||
        ext.isEmpty() || subtitle.size() > MAX_SUBTITLE_SIZE )
    {
        return false;
    }

    if( !m_subtitlesDir )
        m_subtitlesDir.reset( new QTemporaryDir );

    if( !m_subtitlesDir->isValid() )
        return false;

    QTemporaryFile subtitleFile( m_subtitlesDir->path() +
                                 QDir::separator() +
                                 QStringLiteral( "str-sub-XXXXXX." ) + ext );

    return
        subtitleFile.open() &&
        subtitleFile.write( subtitle ) &&
        loadFromFile( &subtitleFile );
}

void QmlVlcSubtitle::eraseLoaded()
{
    if( m_networkReply )
        m_networkReply->abort();

    if( m_subtitleFile )
        m_subtitleFile.reset();

    m_subtitlesDir.reset();

    m_loadedCount = 0;
}
