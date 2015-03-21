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

#include "QmlVlcMmVideoOutput.h"

#include <QAbstractVideoSurface>

QmlVlcMmVideoOutput::QmlVlcMmVideoOutput( vlc::player* player, QObject* parent /*= 0*/ )
    : QObject( parent ), m_player( player ), m_videoSurface( 0 ),
      m_pixelFormat( QVideoFrame::Format_Invalid ),
      m_frameWidth( 0 ), m_frameHeight( 0 ),
      m_frameSize( 0 ), m_pitchSize( 0 ),
      m_UPlaneOffset( 0 ), m_VPlaneOffset( 0 )
{
}

QmlVlcMmVideoOutput::~QmlVlcMmVideoOutput()
{
    //we should force closing vmem here,
    //to avoid pure virtual member call in vlc::basic_vmem_wrapper
    vlc::basic_vmem_wrapper::close();
}

void QmlVlcMmVideoOutput::init()
{
    assert( m_player && m_player->is_open() );
    vlc::basic_vmem_wrapper::open( &( m_player->basic_player() ) );
}

unsigned QmlVlcMmVideoOutput::video_format_cb( char* chroma,
                                               unsigned* width, unsigned* height,
                                               unsigned* pitches, unsigned* lines )
{
    memcpy( chroma, "I420", 4 );
    pitches[0] = *width; if( pitches[0] % 4 ) pitches[0] += 4 - pitches[0] % 4;
    pitches[1] = ( *width + 1 ) / 2; if( pitches[1] % 4 ) pitches[1] += 4 - pitches[1] % 4;
    pitches[2] = pitches[1];

    lines[0] = (*height);
    lines[1] = ( (*height) + 1 ) / 2;
    lines[2] = lines[1];

    m_UPlaneOffset = pitches[0] * lines[0];
    m_VPlaneOffset = m_UPlaneOffset + pitches[1] * lines[1];

    m_pixelFormat = QVideoFrame::Format_YUV420P;
    m_frameWidth = *width;
    m_frameHeight = *height;
    m_frameSize = pitches[0] * lines[0] + pitches[1] * lines[1] + pitches[2] * lines[2];
    m_pitchSize = pitches[0];

    QVideoSurfaceFormat format( QSize( m_frameWidth, m_frameHeight ), m_pixelFormat );

    QMetaObject::invokeMethod( this, "updateSurfaceFormat",
                               Q_ARG( QVideoSurfaceFormat, format ) );

    return 3;
}

void QmlVlcMmVideoOutput::video_cleanup_cb()
{
    m_pixelFormat = QVideoFrame::Format_Invalid;
    m_frameWidth = m_frameHeight = 0;
    m_frameSize = 0;
    m_pitchSize = 0;
    m_UPlaneOffset = m_VPlaneOffset = 0;
    m_videoFrame = QVideoFrame();

    QMetaObject::invokeMethod( this, "cleanupVideoSurface" );
}

void* QmlVlcMmVideoOutput::video_lock_cb( void** planes )
{
    QVideoFrame frame( m_frameSize,
                       QSize( m_frameWidth, m_frameHeight ), m_pitchSize,
                       m_pixelFormat );

    if( frame.map( QAbstractVideoBuffer::WriteOnly ) ) {
        uint8_t* b = frame.bits();
        planes[0] = b;
        planes[1] = b + m_UPlaneOffset;
        planes[2] = b + m_VPlaneOffset;
    } else
        assert( false );

    m_videoFrame = frame;

    return 0;
}

void QmlVlcMmVideoOutput::video_unlock_cb( void* /*picture*/, void *const * /*planes*/ )
{
    if( m_videoFrame.isMapped() )
        m_videoFrame.unmap();
    else
        assert(false);
}

void QmlVlcMmVideoOutput::video_display_cb( void* /*picture*/ )
{
    QMetaObject::invokeMethod( this, "presentFrame",
                               Q_ARG( QVideoFrame, m_videoFrame ) );
}

void QmlVlcMmVideoOutput::updateSurfaceFormat( const QVideoSurfaceFormat& newFormat )
{
    cleanupVideoSurface();

    m_surfaceFormat = newFormat;

    initVideoSurface( newFormat );
}

void QmlVlcMmVideoOutput::presentFrame( const QVideoFrame& newFrame )
{
    if( m_videoSurface )
        m_videoSurface->present( newFrame );
}

void QmlVlcMmVideoOutput::setVideoSurface( QAbstractVideoSurface* s )
{
    if( m_videoSurface == s )
        return;

    cleanupVideoSurface();

    m_videoSurface = s;

    initVideoSurface( m_surfaceFormat );
}

void QmlVlcMmVideoOutput::initVideoSurface( const QVideoSurfaceFormat& format )
{
    assert( !m_videoSurface || !m_videoSurface->isActive() );

    if( m_videoSurface && format.isValid() )
        m_videoSurface->start( format );
}

void QmlVlcMmVideoOutput::cleanupVideoSurface()
{
    if( m_videoSurface && m_videoSurface->isActive() )
        m_videoSurface->stop();
}
