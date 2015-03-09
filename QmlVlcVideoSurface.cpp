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

#include "QmlVlcVideoSurface.h"

#include "SGVlcVideoNode.h"

QmlVlcVideoSurface::QmlVlcVideoSurface()
    : m_fillMode( PreserveAspectFit ), m_source( 0 ), m_frameUpdated( false )
{
    setFlag( QQuickItem::ItemHasContents, true );
}

QmlVlcVideoSurface::~QmlVlcVideoSurface()
{
    setSource( 0 );
}

void QmlVlcVideoSurface::setFillMode( FillMode m )
{
    if( m_fillMode == m )
        return;

    m_fillMode = m;

    update();

    emit fillModeChanged( m );
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
    QRectF srcRect( 0, 0, 1., 1. );

    if( Stretch != fillMode() ) {
        const uint16_t fw = m_frame->width;
        const uint16_t fh = m_frame->height;

        const qreal frameAspect = qreal( fw ) / fh;
        const qreal itemAspect = width() / height();

        if( PreserveAspectFit == fillMode() ) {
            qreal outWidth = width();
            qreal outHeight = height();
            if( frameAspect > itemAspect )
                outHeight = outWidth / frameAspect;
            else if( frameAspect < itemAspect )
                outWidth = outHeight * frameAspect;

            outRect = QRectF( ( width() - outWidth ) / 2, ( height() - outHeight ) / 2,
                               outWidth, outHeight );
        } else if( PreserveAspectCrop == fillMode() ) {
            if( frameAspect > itemAspect ) {
                srcRect.setX( ( 1. - itemAspect / frameAspect ) / 2 );
                srcRect.setWidth( 1. - srcRect.x() - srcRect.x() );
            } else if( frameAspect < itemAspect ) {
                srcRect.setY( ( 1. - frameAspect / itemAspect ) / 2 );
                srcRect.setHeight( 1. - srcRect.y() - srcRect.y() );
            }
        }
    }

    if( m_frameUpdated ) {
        node->setFrame( m_frame );
        m_frameUpdated = false;
    }
    node->setRect( outRect, srcRect );

    return node;
}

void QmlVlcVideoSurface::presentFrame( const QSharedPointer<const QmlVlcI420Frame>& frame )
{
    m_frame = frame;
    m_frameUpdated = true;
    update();
}
