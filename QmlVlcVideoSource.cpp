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

#include "QmlVlcVideoSource.h"

QmlVlcVideoSource::QmlVlcVideoSource( const std::shared_ptr<vlc::player_core>& player, QObject* parent )
    : QObject( parent ), m_videoOutput( new QmlVlcVideoOutput( player ) )
{
}

void QmlVlcVideoSource::classBegin()
{
    m_videoOutput->init();
}

void QmlVlcVideoSource::componentComplete()
{
}

void QmlVlcVideoSource::registerVideoSurface( QmlVlcVideoSurface* s )
{
   m_videoOutput->registerVideoSurface( s );
}

void QmlVlcVideoSource::unregisterVideoSurface( QmlVlcVideoSurface* s )
{
    m_videoOutput->unregisterVideoSurface( s );
}

#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
QAbstractVideoSurface* QmlVlcVideoSource::videoSurface() const
{
    return m_videoOutput->videoSurface();
}

void QmlVlcVideoSource::setVideoSurface( QAbstractVideoSurface* s )
{
    m_videoOutput->setVideoSurface( s );
}
#endif
