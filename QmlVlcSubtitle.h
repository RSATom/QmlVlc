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

#ifndef SUBTITLE_H
#define SUBTITLE_H

#include <QObject>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QByteArray>
#include <QLinkedList>

#include "libvlc_wrapper/vlc_player.h"

class QmlVlcSubtitle : public QObject
{
    Q_OBJECT
public:
    QmlVlcSubtitle( vlc::player& player );

    enum Type {
        Microdvd    = 0,
        Subrip      = 1,
        //Subviewer   = 2,
        //Ssa1        = 3,
        //Ssa2_4      = 4,
        //Ass         = 5,
        //Vplayer     = 6,
        //Sami        = 7,
        //Dvdsubtitle = 8,
        //Mpl2        = 9,
        //Aqt         = 10,
        //Pjs         = 11,
        //Mpsub       = 12,
        //Jacosub     = 13,
        //Psb         = 14,
        //Realtext    = 15,
        Dks         = 16,
        //Subviewer1  = 17,
        Vtt         = 18,
    };

    Q_ENUMS( Type )

    Q_PROPERTY( unsigned count READ get_trackCount )

    Q_PROPERTY( int track READ get_track WRITE set_track )
    Q_PROPERTY( int delay READ get_delay WRITE set_delay )

    unsigned get_trackCount();

    int get_track();
    void set_track( int idx );

    //in milliseconds
    int get_delay();
    void set_delay( int );

    Q_INVOKABLE QString description( unsigned i );

    Q_INVOKABLE void load( const QUrl& );
    Q_INVOKABLE bool loadFromString( const QByteArray& subtitles,
                                     Type type );

    void eraseLoaded();

Q_SIGNALS:
    void loadFinished();
    void loadError();

private Q_SLOTS:
    void networkDataReady();
    void downloadFinished();

private:
    bool loadFromFile( QTemporaryFile* );

private:
    vlc::player& m_player;
    QNetworkAccessManager m_networkManager;
    QNetworkReply* m_networkReply;
    QScopedPointer<QTemporaryDir> m_subtitlesDir;
    QScopedPointer<QTemporaryFile> m_subtitleFile;
    unsigned m_loadedCount;
};

#endif //SUBTITLE_H
