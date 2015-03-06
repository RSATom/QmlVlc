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

#include "SGVlcVideoNode.h"

#include <QtGui/QOpenGLFunctions>

#include "QmlVlcVideoFrame.h"

#if ( QT_VERSION < QT_VERSION_CHECK( 5, 3, 0 ) )
#define F( x ) x
#else
#define F( x ) f->x
#endif

///////////////////////////////////////////////////////////////////////////////
// QSGVlcVideoFrameMaterialShader
char const* const* QSGVlcVideoFrameMaterialShader::attributeNames() const
{
    static const char *names[] = {
        "a_vertex",
        "a_texcoord",
        0
    };
    return names;
}

const char* QSGVlcVideoFrameMaterialShader::vertexShader() const
{
    return
        "uniform highp mat4 matrix;"
        "attribute highp vec4 a_vertex;"
        "attribute highp vec2 a_texCoord;"
        "varying highp vec2 v_texCoord;"
        "void main() {"
        "    v_texCoord = a_texCoord;"
        "    gl_Position = matrix * a_vertex;"
        "}";
}

const char* QSGVlcVideoFrameMaterialShader::fragmentShader() const
{
    return
        "uniform mediump mat4 rgbMatrix;"
        "uniform lowp float opacity;"
        "uniform sampler2D yPlaneTex;"
        "uniform sampler2D uPlaneTex;"
        "uniform sampler2D vPlaneTex;"
        "varying highp vec2 v_texCoord;"
        "void main() {"
        "    mediump float y = texture2D( yPlaneTex, v_texCoord ).r;"
        "    mediump float u = texture2D( uPlaneTex, v_texCoord ).r;"
        "    mediump float v = texture2D( vPlaneTex, v_texCoord ).r;"
        "    gl_FragColor = vec4( y - .0625, u - .5, v - .5, 1. ) * rgbMatrix * opacity;"
        "}";
}

void QSGVlcVideoFrameMaterialShader::initialize()
{
    m_matrixId = program()->uniformLocation("matrix");
    m_rgbMatrixId = program()->uniformLocation("rgbMatrix");
    m_opacityId = program()->uniformLocation("opacity");
    m_yPlaneTexId = program()->uniformLocation("yPlaneTex");
    m_uPlaneTexId = program()->uniformLocation("uPlaneTex");
    m_vPlaneTexId = program()->uniformLocation("vPlaneTex");
}

void QSGVlcVideoFrameMaterialShader::updateState( const RenderState& state,
                                                  QSGMaterial* newMaterial,
                                                  QSGMaterial* /*oldMaterial*/ )
{
    if( state.isOpacityDirty() )
        program()->setUniformValue( m_opacityId, GLfloat( state.opacity() ) );

    if( state.isMatrixDirty() )
        program()->setUniformValue( m_matrixId, state.combinedMatrix() );

    static const QMatrix4x4 rgbMatrix
      ( 1.164f,  1.164f,  1.164f,   .0f,
         .0f,   -0.391f,  2.018f,   .0f,
        1.596f, -0.813f,   .0f,     .0f,
         .0f,     .0f,     .0f,    1.0f );

    program()->setUniformValue( m_rgbMatrixId, rgbMatrix );

    QSGVlcVideoFrameMaterial* fm =
        static_cast<QSGVlcVideoFrameMaterial*>( newMaterial );

    program()->setUniformValue( m_yPlaneTexId, 0 );
    program()->setUniformValue( m_uPlaneTexId, 1 );
    program()->setUniformValue( m_vPlaneTexId, 2 );

    fm->bindPlanes();
}

///////////////////////////////////////////////////////////////////////////////
// QSGVlcVideoFrameMaterial
QSGVlcVideoFrameMaterial::QSGVlcVideoFrameMaterial()
    : m_frame( 0 )
{
    memset( m_planeTexIds, 0, sizeof( m_planeTexIds ) );
    setFlag( Blending, false );
}

QSGVlcVideoFrameMaterial::~QSGVlcVideoFrameMaterial()
{
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    if( m_planeTexIds[0] || m_planeTexIds[1] || m_planeTexIds[2] )
        F( glDeleteTextures( sizeof( m_planeTexIds ) / sizeof( m_planeTexIds[0] ), m_planeTexIds ) );
}

QSGMaterialType* QSGVlcVideoFrameMaterial::type() const
{
    static QSGMaterialType theType;
    return &theType;
}

QSGMaterialShader* QSGVlcVideoFrameMaterial::createShader() const
{
    return new QSGVlcVideoFrameMaterialShader;
}

int QSGVlcVideoFrameMaterial::compare( const QSGMaterial* other ) const
{
    const QSGVlcVideoFrameMaterial* m =
            static_cast<const QSGVlcVideoFrameMaterial*>(other);
    const unsigned texCount = sizeof( m_planeTexIds ) / sizeof( m_planeTexIds[0] );

    for( unsigned i = 0; i < texCount; ++i ) {
        if( m_planeTexIds[i] != m->m_planeTexIds[i] )
            return m_planeTexIds[i] - m->m_planeTexIds[i];
    }

    return 0;
}

void QSGVlcVideoFrameMaterial::setFrame( const QSharedPointer<const QmlVlcI420Frame>& frame )
{
    m_frame = frame;
}

void QSGVlcVideoFrameMaterial::bindPlanes()
{
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    if( 0 == m_planeTexIds[0] && 0 == m_planeTexIds[1] && 0 == m_planeTexIds[2] )
        F( glGenTextures( sizeof( m_planeTexIds ) / sizeof( m_planeTexIds[0] ), m_planeTexIds ) );

    QSharedPointer<const QmlVlcI420Frame> tmpFrame;
    m_frame.swap( tmpFrame );

    if( tmpFrame ) {
        Q_ASSERT( 0 == ( tmpFrame->width & 1 ) && 0 == ( tmpFrame->height & 1 ) );//width and height should be even
        const quint16 tw = tmpFrame->width;
        const quint16 th = tmpFrame->height;

        bindPlane( GL_TEXTURE1, m_planeTexIds[1], tmpFrame->uPlane, tw / 2, th / 2 );
        bindPlane( GL_TEXTURE2, m_planeTexIds[2], tmpFrame->vPlane, tw / 2, th / 2 );
        bindPlane( GL_TEXTURE0, m_planeTexIds[0], tmpFrame->yPlane, tw, th );
    } else {
        bindPlane( GL_TEXTURE1, m_planeTexIds[1], 0, 0, 0 );
        bindPlane( GL_TEXTURE2, m_planeTexIds[2], 0, 0, 0 );
        bindPlane( GL_TEXTURE0, m_planeTexIds[0], 0, 0, 0 );
    }
}

void QSGVlcVideoFrameMaterial::bindPlane( GLenum texUnit, GLuint texId, const void* plane,
                                          quint16 width, quint16 height )
{
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glActiveTexture( texUnit );
    F( glBindTexture( GL_TEXTURE_2D, texId ) );
    if( plane ) {
        F( glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE,
                         width, height, 0,
                         GL_LUMINANCE, GL_UNSIGNED_BYTE, plane ) );
        F( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
        F( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
        F( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
        F( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
    }
}

///////////////////////////////////////////////////////////////////////////////
// SGVlcVideoNode
SGVlcVideoNode::SGVlcVideoNode()
    : m_geometry( QSGGeometry::defaultAttributes_TexturedPoint2D(), 4 )
{
    setGeometry( &m_geometry );
    setMaterial( &m_material );
}

void SGVlcVideoNode::setFrame( const QSharedPointer<const QmlVlcI420Frame>& frame )
{
    m_material.setFrame( frame );
    markDirty( QSGNode::DirtyMaterial );
}

void SGVlcVideoNode::setRect( const QRectF& rect, const QRectF& sourceRect )
{
    QSGGeometry::updateTexturedRectGeometry( &m_geometry, rect, sourceRect );
    markDirty( QSGNode::DirtyGeometry );
}
