#include "SGVlcVideoNode.h"

#include <QtGui/QOpenGLFunctions>

#include "QmlVlcVideoFrame.h"

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
        "uniform sampler2D yPlaneTex;"
        "uniform sampler2D uPlaneTex;"
        "uniform sampler2D vPlaneTex;"
        "varying highp vec2 v_texCoord;"
        "void main() {"
        "    mediump float y = texture2D( yPlaneTex, v_texCoord ).r;"
        "    mediump float u = texture2D( uPlaneTex, v_texCoord ).r;"
        "    mediump float v = texture2D( vPlaneTex, v_texCoord ).r;"
        "    gl_FragColor = vec4( y - .0625, u - .5, v - .5, 1. ) * rgbMatrix;"
        "}";
}

void QSGVlcVideoFrameMaterialShader::initialize()
{
    m_matrixId = program()->uniformLocation("matrix");
    m_rgbMatrixId = program()->uniformLocation("rgbMatrix");
    m_yPlaneTexId = program()->uniformLocation("yPlaneTex");
    m_uPlaneTexId = program()->uniformLocation("uPlaneTex");
    m_vPlaneTexId = program()->uniformLocation("vPlaneTex");
}

void QSGVlcVideoFrameMaterialShader::updateState( const RenderState& state,
                                                  QSGMaterial* newMaterial,
                                                  QSGMaterial* /*oldMaterial*/ )
{
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
}

QSGVlcVideoFrameMaterial::~QSGVlcVideoFrameMaterial()
{
    if( m_planeTexIds[0] || m_planeTexIds[1] || m_planeTexIds[2] )
        glDeleteTextures( sizeof( m_planeTexIds ) / sizeof( m_planeTexIds[0] ), m_planeTexIds );
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
    const auto texCount = sizeof( m_planeTexIds ) / sizeof( m_planeTexIds[0] );

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
    if( 0 == m_planeTexIds[0] && 0 == m_planeTexIds[1] && 0 == m_planeTexIds[2] )
        glGenTextures( sizeof( m_planeTexIds ) / sizeof( m_planeTexIds[0] ), m_planeTexIds );

    if( m_frame ) {
        QMutexLocker lock( m_frame->guard() );

        Q_ASSERT( 0 == ( m_frame->width & 1 ) && 0 == ( m_frame->height & 1 ) );//width and height should be even
        const quint16 tw = m_frame->width;
        const quint16 th = m_frame->height;

        bindPlane( GL_TEXTURE1, m_planeTexIds[1], m_frame->uPlane, tw / 2, th / 2 );
        bindPlane( GL_TEXTURE2, m_planeTexIds[2], m_frame->vPlane, tw / 2, th / 2 );
        bindPlane( GL_TEXTURE0, m_planeTexIds[0], m_frame->yPlane, tw, th );
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
    glBindTexture( GL_TEXTURE_2D, texId );
    if( plane ) {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE,
                      width, height, 0,
                      GL_LUMINANCE, GL_UNSIGNED_BYTE, plane );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
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

void SGVlcVideoNode::setRect( const QRectF& rect )
{
    m_geometry.updateTexturedRectGeometry( &m_geometry, rect, QRectF( 0, 0, 1., 1. ) );
    markDirty( QSGNode::DirtyGeometry );
}
