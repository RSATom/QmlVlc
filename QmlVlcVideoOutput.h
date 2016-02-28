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

#pragma once

#include <cassert>
#include <memory>

#include <QQmlParserStatus>

#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
#include <QVideoSurfaceFormat>
#include <QAbstractVideoSurface>
#endif

#ifndef Q_MOC_RUN
#include "libvlc_wrapper/vlc_player.h"

#include <vmem2.h>
#endif // Q_MOC_RUN

#include "QmlVlcVideoFrame.h"

class QmlVlcVideoSurface; //#include "QmlVlcVideoSurface.h"

class QmlVlcVideoOutput : public QObject
{
    Q_OBJECT
public:
    explicit QmlVlcVideoOutput( const std::shared_ptr<vlc::player_core>& player,
                                QObject* parent = 0 );
    void init();
    ~QmlVlcVideoOutput();

    void registerVideoSurface( QmlVlcVideoSurface* s );
    void unregisterVideoSurface( QmlVlcVideoSurface* s );

    QList<QmlVlcVideoSurface*> attachedSurfaces() const
        { return m_attachedSurfaces; }

    std::shared_ptr<const QmlVlcI420Frame> renderFrame() const
        { return m_renderFrame; }

#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
    QAbstractVideoSurface* videoSurface() const
        { return m_videoSurface; }
    void setVideoSurface( QAbstractVideoSurface* s );
#endif

private:
    Q_INVOKABLE void frameUpdated();

#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
    Q_INVOKABLE void updateSurfaceFormat( const QVideoSurfaceFormat& newFormat );
    Q_INVOKABLE void cleanupVideoSurface();

    void initVideoSurface( const QVideoSurfaceFormat& format );
#endif

private:
    static bool vmem2_setup( void* opaque, vmem2_video_format_t* format );
    static bool vmem2_lock( void* opaque, vmem2_planes_t* planes );
    static void vmem2_unlock( void* opaque, const vmem2_planes_t* planes );
    static void vmem2_display( void* opaque, const vmem2_planes_t* planes );
    static void vmem2_cleanup( void* opaque );

    bool setup( vmem2_video_format_t* format );
    bool lock( vmem2_planes_t* planes );
    void unlock( const vmem2_planes_t* planes );
    void display( const vmem2_planes_t* planes );
    void cleanup();

private:
    std::shared_ptr<vlc::player_core> m_player;

    QList<QmlVlcVideoSurface*> m_attachedSurfaces;

    std::deque<std::shared_ptr<QmlVlcI420Frame> > m_frames;
    std::list<std::shared_ptr<QmlVlcI420Frame> > m_lockedFrames;
    std::shared_ptr<QmlVlcI420Frame> m_renderFrame;

#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
    QVideoSurfaceFormat m_surfaceFormat;
    QAbstractVideoSurface* m_videoSurface;
#endif
};
