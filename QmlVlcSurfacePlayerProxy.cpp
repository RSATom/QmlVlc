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

#include "QmlVlcSurfacePlayerProxy.h"

#include "QmlVlcVideoSurface.h"

QmlVlcSurfacePlayerProxy::QmlVlcSurfacePlayerProxy( vlc::player* player, QObject* parent )
    : QmlVlcPlayerProxy( player, parent ),
      m_videoOutput( new QmlVlcVideoOutput( player ) )
{
}

void QmlVlcSurfacePlayerProxy::classBegin()
{
    QmlVlcPlayerProxy::classBegin();

    m_videoOutput->init();
}

QmlVlcSurfacePlayerProxy::~QmlVlcSurfacePlayerProxy()
{
}

void QmlVlcSurfacePlayerProxy::swap( QmlVlcSurfacePlayerProxy* pp )
{
    if( !pp || pp == this )
        return;

    QList<QmlVlcVideoSurface*> surfaces = m_videoOutput->attachedSurfaces();
    QList<QmlVlcVideoSurface*> ppSurfaces = pp->m_videoOutput->attachedSurfaces();

    for( auto* s: surfaces ) {
        s->setSource( nullptr );
    }
    for( auto* s: ppSurfaces ) {
        s->setSource( nullptr );
    }

    player().swap( &pp->player() );

    m_videoOutput.swap( pp->m_videoOutput );

    for( auto* s: surfaces ) {
        s->setSource( pp );
    }
    for( auto* s: ppSurfaces ) {
        s->setSource( this );
    }
}
