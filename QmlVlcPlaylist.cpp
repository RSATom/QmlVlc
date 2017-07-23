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

#include "QmlVlcPlaylist.h"

#include <QStringList>
#include <QQmlEngine>

#include "QmlVlcConfig.h"
#include "QmlVlcPlayerProxy.h"

QmlVlcPlaylist::QmlVlcPlaylist( QmlVlcPlayerProxy* owner )
    : m_owner( owner )
{
    set_mode( Normal );
}

vlc::playlist_player_core& QmlVlcPlaylist::player()
{
    return m_owner->player();
}

unsigned int QmlVlcPlaylist::get_itemCount()
{
    return player().item_count();
}

bool QmlVlcPlaylist::get_isPlaying()
{
    return player().is_playing();
}

int QmlVlcPlaylist::get_current()
{
    return player().current_item();
}

void QmlVlcPlaylist::set_current( unsigned int idx )
{
    player().set_current( idx );
}

QmlVlcPlaylist::Mode QmlVlcPlaylist::get_mode()
{
    return static_cast<Mode>( player().get_playback_mode() );
}

void QmlVlcPlaylist::set_mode( QmlVlcPlaylist::Mode mode )
{
    if( mode > vlc::mode_last )
        return;

    return player().set_playback_mode( (vlc::playback_mode_e) mode );
}

int QmlVlcPlaylist::itemsCount( ItemsProperty_t* p )
{
    QmlVlcPlaylist* pl = static_cast<QmlVlcPlaylist*>( p->object );
    return pl->get_itemCount();
}

QmlVlcMedia* QmlVlcPlaylist::getItem( ItemsProperty_t* p, int index )
{
    QmlVlcPlaylist* pl = static_cast<QmlVlcPlaylist*>( p->object );

    QmlVlcMediaMedia* md =
        new QmlVlcMediaMedia( pl->m_owner, pl->player().get_media( index ) );
    QQmlEngine::setObjectOwnership( md, QQmlEngine::JavaScriptOwnership );

    return md;
}

void QmlVlcPlaylist::itemsClear( ItemsProperty_t* p )
{
    QmlVlcPlaylist* pl = static_cast<QmlVlcPlaylist*>( p->object );
    return pl->clear();
}

QQmlListProperty<QmlVlcMedia> QmlVlcPlaylist::get_items()
{
    return
        ItemsProperty_t( this, 0, 0,
                         itemsCount, getItem, itemsClear );
}

int QmlVlcPlaylist::add( const QString& mrl )
{
    return player().add_media( mrl.toUtf8().data() );
}

int QmlVlcPlaylist::add(const QUrl &mrl)
{
    return player().add_media( mrl.toString().toUtf8().data() );
}

int QmlVlcPlaylist::add( QmlVlcMedia* media )
{
    if( !media )
        return -1;

    return player().add_media( media->get_media() );
}

int QmlVlcPlaylist::addWithOptions( const QString& mrl, const QStringList& options )
{
    QList<QByteArray> bufStorage;
    std::vector<const char*> trusted_opts;
    std::vector<const char*> untrusted_opts;

    for( int i = 0; i < options.size(); ++i ) {
        QByteArray& buf = *bufStorage.insert( bufStorage.end(), options[i].toUtf8() );
        if( QmlVlcConfig::instance().isOptionTrusted( options[i] ) )
            trusted_opts.push_back( buf.data() );
        else
            untrusted_opts.push_back( buf.data() );
    }

    const char** untrusted_optv = untrusted_opts.empty() ? 0 : &untrusted_opts[0];
    const char** trusted_optv = trusted_opts.empty() ? 0 : &trusted_opts[0];
    return player().add_media( mrl.toUtf8().data(),
                               untrusted_opts.size(), untrusted_optv,
                               trusted_opts.size(), trusted_optv );
}

void QmlVlcPlaylist::play()
{
    player().play();
}

bool QmlVlcPlaylist::playItem( unsigned int idx )
{
    return player().play( idx );
}

void QmlVlcPlaylist::pause()
{
    player().pause();
}

void QmlVlcPlaylist::togglePause()
{
    if ( player().is_playing() ) player().pause();
    else player().play();
}

void QmlVlcPlaylist::stop()
{
    player().stop();
}

void QmlVlcPlaylist::next()
{
    player().next();
}

void QmlVlcPlaylist::prev()
{
    player().prev();
}

void QmlVlcPlaylist::clear()
{
    player().clear_items();
}

bool QmlVlcPlaylist::removeItem( unsigned idx )
{
    return player().delete_item( idx );
}

void QmlVlcPlaylist::advanceItem( unsigned idx, int count )
{
    player().advance_item( idx, count );
}
