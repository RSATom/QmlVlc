#include "QmlVlcVideoSurface.h"

#include "SGVlcVideoNode.h"

QmlVlcVideoSurface::QmlVlcVideoSurface()
    : m_source( 0 ), m_frameUpdated( false )
{
    setFlag( QQuickItem::ItemHasContents, true );
}

QmlVlcVideoSurface::~QmlVlcVideoSurface()
{
    setSource( 0 );
}

void QmlVlcVideoSurface::setSource( QmlVlcSurfacePlayerProxy* source )
{
    if( source == m_source.data() )
        return;

    if( m_source )
        m_source->unregisterVideoSurface( this );

    m_source = source;

    if( m_source )
        m_source->registerVideoSurface( this );

    Q_EMIT sourceChanged();
}

QSGNode* QmlVlcVideoSurface::updatePaintNode( QSGNode* oldNode,
                                              UpdatePaintNodeData* /*data*/ )
{
    SGVlcVideoNode* node = static_cast<SGVlcVideoNode*>( oldNode );
    if( !m_frame ) {
        delete node;
        return 0;
    }

    if( !node )
        node = new SGVlcVideoNode;

    QRectF outRect( 0, 0, width(), height() );

    const uint16_t fw = m_frame->width;
    const uint16_t fh = m_frame->height;

    const qreal frameAspect = qreal( fw ) / fh;
    const qreal itemAspect = width() / height();
    qreal outWidth = width();
    qreal outHeight = height();

    if( frameAspect > itemAspect )
        outHeight = outWidth / frameAspect;
    else if( frameAspect < itemAspect )
        outWidth = outHeight * frameAspect;

    outRect = QRectF( ( width() - outWidth ) / 2, ( height() - outHeight ) / 2,
                       outWidth, outHeight );

    if( m_frameUpdated ) {
        node->setFrame( m_frame );
        m_frameUpdated = false;
    }
    node->setRect( outRect );

    return node;
}

void QmlVlcVideoSurface::presentFrame( const QSharedPointer<const QmlVlcI420Frame>& frame )
{
    m_frame = frame;
    m_frameUpdated = true;
    update();
}
