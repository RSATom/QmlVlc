#ifndef QMLVLCVIDEOOUTPUT_H
#define QMLVLCVIDEOOUTPUT_H

#include <cassert>

#include <QObject>
#include <QSharedPointer>

#ifndef Q_MOC_RUN
#include <libvlc_wrapper/vlc_player.h>
#include <libvlc_wrapper/vlc_vmem.h>
#endif // Q_MOC_RUN

#include "QmlVlcVideoFrame.h"

#include <QQmlParserStatus>

class QmlVlcVideoSurface; //#include "QmlVlcVideoSurface.h"

class QmlVlcVideoOutput
    : public QObject,
      private vlc::basic_vmem_wrapper,
      public QQmlParserStatus
{
    Q_OBJECT
public:
    explicit QmlVlcVideoOutput( vlc::player* player, QObject *parent = 0 );
    ~QmlVlcVideoOutput();

    void registerVideoSurface( QmlVlcVideoSurface* s );
    void unregisterVideoSurface( QmlVlcVideoSurface* s );

private:
    Q_INVOKABLE void frameUpdated();

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

protected:
    vlc::player& player() { assert( m_player ); return *m_player; }

protected:
    virtual void classBegin();
    virtual void componentComplete();

private:
    vlc::player *const m_player;

    QList<QmlVlcVideoSurface*> m_attachedSurfaces;

    QSharedPointer<QmlVlcI420Frame> m_frame;
};

#endif //QMLVLCVIDEOOUTPUT_H
