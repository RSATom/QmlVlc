#include "QmlVlcVideoOutput.h"

QmlVlcVideoOutput::QmlVlcVideoOutput( vlc::player* player, QObject *parent /*= 0*/)
    : QObject( parent ), m_player( player ), m_videoSurface( 0 ),
      m_UPlaneOffset( 0 ), m_VPlaneOffset( 0 )
{
    assert( player && m_player->is_open() );
    vlc::basic_vmem_wrapper::open( &( m_player->basic_player() ) );
}

unsigned QmlVlcVideoOutput::video_format_cb( char *chroma,
                                             unsigned *width, unsigned *height,
                                             unsigned *pitches, unsigned *lines )
{
    QMutexLocker locker( &m_frameGuard );

    memcpy( chroma, "I420", 4 );
    pitches[0] = *width; if( pitches[0] % 4 ) pitches[0] += 4 - pitches[0] % 4;
    pitches[1] = ( *width + 1 ) / 2; if( pitches[1] % 4 ) pitches[1] += 4 - pitches[1] % 4;
    pitches[2] = pitches[1];

    lines[0] = (*height);
    lines[1] = ( (*height) + 1 ) / 2;
    lines[2] = lines[1];

    m_UPlaneOffset = pitches[0] * lines[0];
    m_VPlaneOffset = m_UPlaneOffset + pitches[1] * lines[1];

    m_videoFrame =
        QVideoFrame( pitches[0] * lines[0] + pitches[1] * lines[1] + pitches[2] * lines[2],
                     QSize( *width, *height ), (*pitches),
                     QVideoFrame::Format_YUV420P );

    m_surfaceFormat = QVideoSurfaceFormat( QSize( *width, *height ), m_videoFrame.pixelFormat() );
    QMetaObject::invokeMethod( this, "initVideoSurface" );

    return 3;
}

void QmlVlcVideoOutput::video_cleanup_cb()
{
    QMutexLocker locker( &m_frameGuard );

    Q_ASSERT( !m_videoFrame.isMapped() );

    m_UPlaneOffset = m_VPlaneOffset = 0;

    QMetaObject::invokeMethod( this, "cleanupVideoSurface" );
}

void* QmlVlcVideoOutput::video_lock_cb( void **planes )
{
    m_frameGuard.lock();

    if( m_videoFrame.map( QAbstractVideoBuffer::WriteOnly ) ) {
        uint8_t* b = m_videoFrame.bits();
        planes[0] = b;
        planes[1] = b + m_UPlaneOffset;
        planes[2] = b + m_VPlaneOffset;
    } else {
        planes[0] = 0;
        planes[1] = 0;
        planes[2] = 0;
    }

    return 0;
}

void QmlVlcVideoOutput::video_unlock_cb( void* /*picture*/, void *const * /*planes*/ )
{
    if( m_videoFrame.isMapped() )
        m_videoFrame.unmap();

    m_frameGuard.unlock();
}

void QmlVlcVideoOutput::video_display_cb( void* /*picture*/ )
{
    QMetaObject::invokeMethod( this, "updateFrame" );
}

void QmlVlcVideoOutput::setVideoSurface( QAbstractVideoSurface* s )
{
    if( m_videoSurface == s )
        return;

    cleanupVideoSurface();

    m_videoSurface = s;

    initVideoSurface();
}

void QmlVlcVideoOutput::initVideoSurface()
{
    if( !m_videoSurface )
        return;

    cleanupVideoSurface();

    QMutexLocker locker( &m_frameGuard );
    if( m_surfaceFormat.isValid() )
        m_videoSurface->start( m_surfaceFormat );
}

void QmlVlcVideoOutput::updateFrame()
{
    QMutexLocker locker( &m_frameGuard );

    if( m_videoSurface ) {
        m_videoSurface->present( m_videoFrame );
    }
}

void QmlVlcVideoOutput::cleanupVideoSurface()
{
    if( m_videoSurface && m_videoSurface->isActive() )
        m_videoSurface->stop();
}
