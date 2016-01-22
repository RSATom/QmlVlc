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
#include "libvlc_wrapper/vlc_vmem.h"
#endif // Q_MOC_RUN

#include "QmlVlcVideoFrame.h"

class QmlVlcVideoSurface; //#include "QmlVlcVideoSurface.h"

class QmlVlcVideoOutput
    : public QObject,
      private vlc::basic_vmem_wrapper
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
    //for libvlc_video_set_format_callbacks
    virtual unsigned video_format_cb( char *chroma,
                                      unsigned *width, unsigned *height,
                                      unsigned *pitches, unsigned *lines );
    virtual void video_cleanup_cb();
    //end (for libvlc_video_set_format_callbacks)

    //for libvlc_video_set_callbacks
    virtual void* video_lock_cb( void **planes );
    virtual void  video_unlock_cb( void *picture, void *const *planes );
    virtual void  video_display_cb( void *picture );
    //end (for libvlc_video_set_callbacks)

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
