/*******************************************************************************
* Copyright © 2015, Sergey Radionov <rsatom_gmail.com>
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

#include "QmlVlcMediaListPlayerProxy.h"

QmlVlcMediaListPlayerProxy::QmlVlcMediaListPlayerProxy( const std::shared_ptr<vlc::media_list_player>& player, QObject* parent )
    : QmlVlcPlayerProxy( player, parent )
{
}

QmlVlcMediaListPlayerProxy::~QmlVlcMediaListPlayerProxy()
{
}

static void LogEvent( const libvlc_event_t* e )
{
    switch ( e->type ) {
    case libvlc_MediaListItemAdded:
        qDebug() << "MediaListItemAdded";
        break;
    case libvlc_MediaListWillAddItem:
        qDebug() << "MediaListWillAddItem";
        break;
    case libvlc_MediaListItemDeleted:
        qDebug() << "MediaListItemDeleted";
        break;
    case libvlc_MediaListWillDeleteItem:
        qDebug() << "MediaListWillDeleteItem";
        break;
    };
}

void QmlVlcMediaListPlayerProxy::handleLibvlcEvent( const LibvlcEvent& event )
{
    //it's highly possible libvlc_event_t will have wrong pointers inside at this point
    const libvlc_event_t* e = &event._libvlcEvent;

    //LogEvent( e );

    switch ( e->type ) {
    case libvlc_MediaListItemAdded:
        Q_EMIT mediaListItemAdded( e->u.media_list_item_added.index );
        break;
    case libvlc_MediaListWillAddItem:
        Q_EMIT mediaListWillAddItem( e->u.media_list_will_add_item.index );
        break;
    case libvlc_MediaListItemDeleted:
        Q_EMIT mediaListItemDeleted( e->u.media_list_item_deleted.index );
        break;
    case libvlc_MediaListWillDeleteItem:
        Q_EMIT mediaListWillDeleteItem( e->u.media_list_will_delete_item.index );
        break;
    case libvlc_MediaListPlayerPlayed:
        Q_EMIT mediaListPlayerPlayed();
        break;
    case libvlc_MediaListPlayerNextItemSet:
        Q_EMIT mediaListPlayerNextItemSet();
        break;
    case libvlc_MediaListPlayerStopped:
        Q_EMIT mediaListPlayerStopped();
        break;
    default:
        QmlVlcPlayerProxy::handleLibvlcEvent( event );
        break;
    };
}
