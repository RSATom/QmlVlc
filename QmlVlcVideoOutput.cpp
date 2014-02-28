#include "QmlVlcVideoOutput.h"

#include <functional>

#include "QmlVlcVideoSurface.h"

QmlVlcVideoOutput::QmlVlcVideoOutput( vlc::player* player, QObject *parent /*= 0*/)
    : QObject( parent ), m_player( player )
{
}

QmlVlcVideoOutput::~QmlVlcVideoOutput()
{
}

void QmlVlcVideoOutput::classBegin()
{
    assert( m_player && m_player->is_open() );
    vlc::basic_vmem_wrapper::open( &( m_player->basic_player() ) );
}

void QmlVlcVideoOutput::componentComplete()
{

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

    m_frame.reset( new QmlVlcI420Frame );

    m_frame->frameBuf.resize( pitches[0] * lines[0] + pitches[1] * lines[1] + pitches[2] * lines[2] );

    m_frame->width = evenWidth;
    m_frame->height = evenHeight;

    char* fb = m_frame->frameBuf.data();

    m_frame->yPlane = fb;
    m_frame->yPlaneSize = pitches[0] * lines[0];

    m_frame->uPlane = fb + m_frame->yPlaneSize;
    m_frame->uPlaneSize = pitches[1] * lines[1];

    m_frame->vPlane = fb + m_frame->yPlaneSize + m_frame->uPlaneSize;
    m_frame->vPlaneSize = pitches[2] * lines[2];

    return 3;
}

void QmlVlcVideoOutput::video_cleanup_cb()
{
    m_frame.clear();

    QMetaObject::invokeMethod( this, "frameUpdated" );
}

void* QmlVlcVideoOutput::video_lock_cb( void **planes )
{
    Q_ASSERT( m_frame );
    m_frame->guard()->lock();

    planes[0] = m_frame->yPlane;
    planes[1] = m_frame->uPlane;
    planes[2] = m_frame->vPlane;

    return 0;
}

void QmlVlcVideoOutput::video_unlock_cb( void* /*picture*/, void *const * /*planes*/ )
{
    Q_ASSERT( m_frame );
    m_frame->guard()->unlock();
}

void QmlVlcVideoOutput::video_display_cb( void* /*picture*/ )
{
    QMetaObject::invokeMethod( this, "frameUpdated" );
}

void QmlVlcVideoOutput::frameUpdated()
{
    //protect frame from destruction while we will process it...
    QSharedPointer<const QmlVlcI420Frame> frame = m_frame;

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
