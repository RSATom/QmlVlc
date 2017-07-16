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

#pragma once

#include <QObject>
#include <QQmlListProperty>

#include "libvlc_wrapper/vlc_player.h"

class QmlVlcPlayerProxy; //#include "QmlVlcPlayerProxy.h"
#include "QmlVlcMedia.h"

class QmlVlcPlaylist : public QObject
{
    Q_OBJECT
public:
    QmlVlcPlaylist( QmlVlcPlayerProxy* owner );

    typedef QQmlListProperty<QmlVlcMedia> ItemsProperty_t;

    enum Mode {
        Single = vlc::mode_single,
        Normal = vlc::mode_normal,
        Loop   = vlc::mode_loop,
    };
    Q_ENUMS( Mode )

    Q_PROPERTY( unsigned itemCount READ get_itemCount )
    Q_PROPERTY( bool isPlaying READ get_isPlaying )

    Q_PROPERTY( int currentItem READ get_current WRITE set_current NOTIFY currentItemChanged )
    Q_PROPERTY( Mode mode READ get_mode WRITE set_mode )

    Q_PROPERTY( QQmlListProperty<QmlVlcMedia> items READ get_items )

    unsigned get_itemCount();
    bool get_isPlaying();

    int get_current();
    void set_current( unsigned idx );

    Mode get_mode();
    void set_mode( Mode );

    ItemsProperty_t get_items();

    Q_INVOKABLE int add( const QString& mrl );
    Q_INVOKABLE int add( const QUrl& mrl );
    Q_INVOKABLE int add( QmlVlcMedia* media );
    Q_INVOKABLE int addWithOptions( const QString& mrl, const QStringList& options );

    Q_INVOKABLE void play();
    Q_INVOKABLE bool playItem( unsigned idx );
    Q_INVOKABLE void pause();
    Q_INVOKABLE void togglePause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void next();
    Q_INVOKABLE void prev();
    Q_INVOKABLE void clear();
    Q_INVOKABLE bool removeItem( unsigned idx );
    Q_INVOKABLE void advanceItem( unsigned idx, int count );

Q_SIGNALS:
    //will emit from QmlPlayerProxy
    void currentItemChanged();

private:
    static int itemsCount( ItemsProperty_t* );
    static QmlVlcMedia* getItem( ItemsProperty_t*, int index );
    static void itemsClear( ItemsProperty_t* );

    inline vlc::playlist_player_core& player();

private:
    QmlVlcPlayerProxy *const m_owner;
};
