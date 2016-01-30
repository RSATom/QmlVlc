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

#include "QmlVlcVideoOutput.h"

#include <functional>

#include "QmlVlcVideoSurface.h"

#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
class MmVideoBuffer : public QAbstractVideoBuffer
{
public:
    MmVideoBuffer( const QmlVlcVideoOutput* );

    uchar* map( MapMode mode, int* numBytes, int* bytesPerLine) override;
    MapMode mapMode() const override;
    void unmap() override;

private:
    QPointer<const QmlVlcVideoOutput> m_source;
    MapMode m_mode;
    std::shared_ptr<const QmlVlcI420Frame> m_renderFrame;
};

MmVideoBuffer::MmVideoBuffer( const QmlVlcVideoOutput* source )
    : QAbstractVideoBuffer( HandleType( NoHandle ) ),
      m_source( source ), m_mode( NotMapped )
{
}

uchar* MmVideoBuffer::map( MapMode mode, int* numBytes, int* bytesPerLine )
{
    Q_ASSERT( ReadOnly == mode );

    if( !m_source )
        return nullptr;

    m_mode = ReadOnly;
    m_renderFrame = m_source->renderFrame();
    if( m_renderFrame ) {
        *numBytes = m_renderFrame->frameBuf.size();
        *bytesPerLine = m_renderFrame->yPlaneSize / m_renderFrame->height;
        return (uchar*) m_renderFrame->frameBuf.data();
    } else {
        *numBytes = 0;
        *bytesPerLine = 0;
        return nullptr;
    }
}

QAbstractVideoBuffer::MapMode MmVideoBuffer::mapMode() const
{
    return m_mode;
}

void MmVideoBuffer::unmap()
{
    m_mode = NotMapped;
    m_renderFrame.reset();
}
#endif

QmlVlcVideoOutput::QmlVlcVideoOutput( const std::shared_ptr<vlc::player_core>& player,
                                      QObject* parent /*= 0*/ )
    : QObject( parent ), m_player( player )
#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
    , m_videoSurface( nullptr )
#endif
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

std::shared_ptr<QmlVlcI420Frame> cloneFrame( const std::shared_ptr<QmlVlcI420Frame>& from )
{
    std::shared_ptr<QmlVlcI420Frame> newFrame( new QmlVlcI420Frame );

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


    std::shared_ptr<QmlVlcI420Frame>& frame =
            *m_frames.emplace( m_frames.end(), new QmlVlcI420Frame );

    frame->frameBuf.resize( pitches[0] * lines[0] + pitches[1] * lines[1] + pitches[2] * lines[2] );

    frame->width = evenWidth;
    frame->height = evenHeight;

    char* fb = frame->frameBuf.data();

    frame->yPlane = fb;
    frame->yPlaneSize = pitches[0] * lines[0];

    frame->uPlane = fb + frame->yPlaneSize;
    frame->uPlaneSize = pitches[1] * lines[1];

    frame->vPlane = fb + frame->yPlaneSize + frame->uPlaneSize;
    frame->vPlaneSize = pitches[2] * lines[2];

#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
    QVideoSurfaceFormat format( QSize( evenWidth, evenHeight ), QVideoFrame::Format_YUV420P );

    QMetaObject::invokeMethod( this, "updateSurfaceFormat",
                               Q_ARG( QVideoSurfaceFormat, format ) );
#endif

    return 3;
}

void QmlVlcVideoOutput::video_cleanup_cb()
{
    m_renderFrame.reset();
    m_lockedFrames.clear();
    m_frames.clear();

    QMetaObject::invokeMethod( this, "frameUpdated" );

#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
    QMetaObject::invokeMethod( this, "cleanupVideoSurface" );
#endif
}

void* QmlVlcVideoOutput::video_lock_cb( void** planes )
{
    auto frameIt = m_frames.begin();
    for( ; frameIt != m_frames.end() && frameIt->use_count() > 1; ++frameIt );

    if( frameIt == m_frames.end() )
        frameIt = m_frames.emplace( m_frames.end(), cloneFrame( m_frames.front() ) );

    std::shared_ptr<QmlVlcI420Frame>& frame = *frameIt;
    planes[0] = frame->yPlane;
    planes[1] = frame->uPlane;
    planes[2] = frame->vPlane;

    m_lockedFrames.emplace_back( frame );

    return reinterpret_cast<void*>( frameIt - m_frames.begin() );
}

void QmlVlcVideoOutput::video_unlock_cb( void* picture, void *const * /*planes*/ )
{
    auto frameNo = reinterpret_cast<decltype( m_frames )::size_type>( picture );
    if( frameNo >= m_frames.size() ) {
        return;
    }

    std::shared_ptr<QmlVlcI420Frame>& frame = m_frames[frameNo];

    m_lockedFrames.erase( std::find( m_lockedFrames.begin(), m_lockedFrames.end(), frame ) );
}

void QmlVlcVideoOutput::video_display_cb( void* picture )
{
    auto frameNo = reinterpret_cast<decltype( m_frames )::size_type>( picture );
    if( frameNo >= m_frames.size() ) {
        assert( false );
        return;
    }

    std::shared_ptr<QmlVlcI420Frame>& frame = m_frames[frameNo];

    m_renderFrame = frame;

    QMetaObject::invokeMethod( this, "frameUpdated" );
}

void QmlVlcVideoOutput::frameUpdated()
{
    //convert to shared pointer to const frame to avoid crash
    std::shared_ptr<const QmlVlcI420Frame> frame = m_renderFrame;

    std::for_each( m_attachedSurfaces.begin(), m_attachedSurfaces.end(),
                   std::bind2nd( std::mem_fun( &QmlVlcVideoSurface::presentFrame ), frame ) );

#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
    if( m_videoSurface ) {
        QVideoFrame frame( new MmVideoBuffer( this ),
                           m_surfaceFormat.frameSize(),
                           m_surfaceFormat.pixelFormat() );
        m_videoSurface->present( frame );
    }
#endif
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

#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
void QmlVlcVideoOutput::initVideoSurface( const QVideoSurfaceFormat& format )
{
    assert( !m_videoSurface || !m_videoSurface->isActive() );

    if( m_videoSurface && format.isValid() )
        m_videoSurface->start( format );
}

void QmlVlcVideoOutput::cleanupVideoSurface()
{
    if( m_videoSurface && m_videoSurface->isActive() )
        m_videoSurface->stop();
}

void QmlVlcVideoOutput::updateSurfaceFormat( const QVideoSurfaceFormat& newFormat )
{
    cleanupVideoSurface();

    m_surfaceFormat = newFormat;

    initVideoSurface( newFormat );
}

void QmlVlcVideoOutput::setVideoSurface( QAbstractVideoSurface* s )
{
    if( m_videoSurface == s )
        return;

    cleanupVideoSurface();

    m_videoSurface = s;

    initVideoSurface( m_surfaceFormat );
}
#endif
