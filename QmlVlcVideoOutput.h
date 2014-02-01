#ifndef QMLVLCVIDEOOUTPUT_H
#define QMLVLCVIDEOOUTPUT_H

#include <QObject>
class QAbstractVideoSurface; //#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>

#ifndef Q_MOC_RUN
#include <libvlc_wrapper/vlc_player.h>
#include <libvlc_wrapper/vlc_vmem.h>
#endif // Q_MOC_RUN

class QmlVlcVideoOutput
    : public QObject,
      private vlc::basic_vmem_wrapper
{
    Q_OBJECT
public:
    explicit QmlVlcVideoOutput( vlc::player* player, QObject *parent = 0 );

    Q_PROPERTY( QAbstractVideoSurface* videoSurface READ videoSurface WRITE setVideoSurface )

    QAbstractVideoSurface* videoSurface() const { return m_videoSurface; }
    void setVideoSurface( QAbstractVideoSurface* s );

private:
    Q_INVOKABLE void initVideoSurface( );
    Q_INVOKABLE void updateFrame();
    Q_INVOKABLE void cleanupVideoSurface();

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

private:
    vlc::player *const m_player;
    QAbstractVideoSurface* m_videoSurface;

    QMutex m_frameGuard;
    unsigned m_UPlaneOffset;
    unsigned m_VPlaneOffset;
    QVideoSurfaceFormat m_surfaceFormat;
    QVideoFrame m_videoFrame;
};

#endif //QMLVLCVIDEOOUTPUT_H
