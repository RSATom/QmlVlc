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

    void setRect( const QRectF& rect, const QRectF& sourceRect );
    void setFrame( const QSharedPointer<const QmlVlcI420Frame>& frame );

private:
    QSGGeometry m_geometry;
    QSGVlcVideoFrameMaterial m_material;
};

#endif
