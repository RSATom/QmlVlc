#ifndef SGVLCVIDEONODE_H
#define SGVLCVIDEONODE_H

#include <QSharedPointer>

#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGMaterial>

struct QmlVlcI420Frame;//#include "QmlVlcVideoFrame.h"

///////////////////////////////////////////////////////////////////////////////
// QSGVlcVideoFrameMaterialShader
class QSGVlcVideoFrameMaterialShader : public QSGMaterialShader
{
public:
    virtual char const* const* attributeNames() const;
    virtual const char* vertexShader() const;
    virtual const char* fragmentShader() const;
    virtual void initialize();

    virtual void updateState( const RenderState& state,
                              QSGMaterial* newMaterial,
                              QSGMaterial* oldMaterial );

private:
    int m_matrixId;
    int m_rgbMatrixId;
    int m_opacityId;
    int m_yPlaneTexId;
    int m_uPlaneTexId;
    int m_vPlaneTexId;
};

///////////////////////////////////////////////////////////////////////////////
// QSGVlcVideoFrameMaterial
class QSGVlcVideoFrameMaterial : public QSGMaterial
{
public:
    QSGVlcVideoFrameMaterial();
    ~QSGVlcVideoFrameMaterial();

    virtual QSGMaterialType* type() const;
    virtual QSGMaterialShader* createShader() const;
    virtual int compare( const QSGMaterial *other ) const;

    void setFrame( const QSharedPointer<const QmlVlcI420Frame>& frame );

    void bindPlanes();

private:
    void bindPlane( GLenum texUnit, GLuint texId, const void* plane,
                    quint16 width, quint16 height );

private:
    QSharedPointer<const QmlVlcI420Frame> m_frame;
    GLuint m_planeTexIds[3];
};

///////////////////////////////////////////////////////////////////////////////
// SGVlcVideoNode
class SGVlcVideoNode : public QSGGeometryNode
{
public:
    SGVlcVideoNode();

    void setRect( const QRectF& rect );
    void setFrame( const QSharedPointer<const QmlVlcI420Frame>& frame );

private:
    QSGGeometry m_geometry;
    QSGVlcVideoFrameMaterial m_material;
};

#endif
