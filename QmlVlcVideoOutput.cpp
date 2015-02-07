/*******************************************************************************
* Copyright © 2014, Sergey Radionov <rsatom_gmail.com>
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

#include "QmlVlcVideoOutput.h"

#include <functional>

#include "QmlVlcVideoSurface.h"

QmlVlcVideoOutput::QmlVlcVideoOutput( vlc::player* player, QObject* parent /*= 0*/ )
    : QObject( parent ), m_player( player )
{
}

void QmlVlcVideoOutput::init()
{
    assert( m_player && m_player->is_open() );
    vlc::basic_vmem_wrapper::open( &( m_player->basic_player() ) );
}

QmlVlcVideoOutput::~QmlVlcVideoOutput()
{
    //we should force closing vmem here,
    //to avoid pure virtual member call in vlc::basic_vmem_wrapper
    vlc::basic_vmem_wrapper::close();
}

QSharedPointer<QmlVlcI420Frame> cloneFrame( const QSharedPointer<QmlVlcI420Frame>& from )
{
    QSharedPointer<QmlVlcI420Frame> newFrame( new QmlVlcI420Frame );

    newFrame->frameBuf.resize( from->frameBuf.size() );

    newFrame->width = from->width;
    newFrame->height = from->height;

    char* fb = newFrame->frameBuf.data();

    newFrame->yPlane = fb;
    newFrame->yPlaneSize = from->yPlaneSize;

    newFrame->uPlane = fb + newFrame->yPlaneSize;
    newFrame->uPlaneSize = from->uPlaneSize;

    newFrame->vPlane = fb + newFrame->yPlaneSize + newFrame->uPlaneSize;
    newFrame->vPlaneSize = from->vPlaneSize;

    return newFrame;
}

unsigned QmlVlcVideoOutput::video_format_cb( char *chroma,
                                             unsigned *width, unsigned *height,
                                             unsigned *pitches, unsigned *lines )
{
    memcpy( chroma, "I420", 4 );

    uint16_t evenWidth = *width + ( *width & 1 ? 1 : 0 );
    uint16_t evenHeight = *height + ( *height & 1 ? 1 : 0 );

    pitches[0] = evenWidth; if( pitches[0] % 4 ) pitches[0] += 4 - pitches[0] % 4;
    pitches[1] = evenWidth / 2; if( pitches[1] % 4 ) pitches[1] += 4 - pitches[1] % 4;
    pitches[2] = pitches[1];

    lines[0] = evenHeight;
    lines[1] = evenHeight / 2;
    lines[2] = lines[1];

    m_decodeFrame.reset( new QmlVlcI420Frame );

    m_decodeFrame->frameBuf.resize( pitches[0] * lines[0] + pitches[1] * lines[1] + pitches[2] * lines[2] );

    m_decodeFrame->width = evenWidth;
    m_decodeFrame->height = evenHeight;

    char* fb = m_decodeFrame->frameBuf.data();

    m_decodeFrame->yPlane = fb;
    m_decodeFrame->yPlaneSize = pitches[0] * lines[0];

    m_decodeFrame->uPlane = fb + m_decodeFrame->yPlaneSize;
    m_decodeFrame->uPlaneSize = pitches[1] * lines[1];

    m_decodeFrame->vPlane = fb + m_decodeFrame->yPlaneSize + m_decodeFrame->uPlaneSize;
    m_decodeFrame->vPlaneSize = pitches[2] * lines[2];

    m_renderFrame = cloneFrame( m_decodeFrame );

    return 3;
}

void QmlVlcVideoOutput::video_cleanup_cb()
{
    m_decodeFrame.clear();
    m_renderFrame.clear();

    QMetaObject::invokeMethod( this, "frameUpdated" );
}

void* QmlVlcVideoOutput::video_lock_cb( void **planes )
{
    Q_ASSERT( m_decodeFrame );

    planes[0] = m_decodeFrame->yPlane;
    planes[1] = m_decodeFrame->uPlane;
    planes[2] = m_decodeFrame->vPlane;

    return 0;
}

void QmlVlcVideoOutput::video_unlock_cb( void* /*picture*/, void *const * /*planes*/ )
{
}

void QmlVlcVideoOutput::video_display_cb( void* /*picture*/ )
{
    m_renderFrame.swap( m_decodeFrame );
    QMetaObject::invokeMethod( this, "frameUpdated" );
}

void QmlVlcVideoOutput::frameUpdated()
{
    //convert to shared pointer to const frame to avoid crash
    QSharedPointer<const QmlVlcI420Frame> frame = m_renderFrame;

    std::for_each( m_attachedSurfaces.begin(), m_attachedSurfaces.end(),
                   std::bind2nd( std::mem_fun( &QmlVlcVideoSurface::presentFrame ), frame ) );
}

void QmlVlcVideoOutput::registerVideoSurface( QmlVlcVideoSurface* s )
{
    Q_ASSERT( m_attachedSurfaces.count( s ) <= 1 );

    if( m_attachedSurfaces.contains( s ) )
        return;

    m_attachedSurfaces.append( s );
}

void QmlVlcVideoOutput::unregisterVideoSurface( QmlVlcVideoSurface* s )
{
    Q_ASSERT( m_attachedSurfaces.count( s ) <= 1 );

    m_attachedSurfaces.removeOne( s );
}
