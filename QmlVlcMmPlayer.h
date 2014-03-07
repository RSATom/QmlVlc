#ifndef QMLVLCMMPLAYER_H
#define QMLVLCMMPLAYER_H

#include "QmlVlcPlayerProxy.h"
#include "QmlVlcMmVideoOutput.h"

class QmlVlcMmPlayer
    : public QmlVlcPlayerProxy
{
    Q_OBJECT
public:
    explicit QmlVlcMmPlayer( QObject* parent = 0 );
    ~QmlVlcMmPlayer();

    Q_PROPERTY( QAbstractVideoSurface* videoSurface READ videoSurface WRITE setVideoSurface )

    QAbstractVideoSurface* videoSurface() const
        { return m_videoOutput.videoSurface(); }
    void setVideoSurface( QAbstractVideoSurface* s )
        { m_videoOutput.setVideoSurface( s ); }

private:
    libvlc_instance_t* m_libvlc;
    vlc::player m_player;
    QmlVlcMmVideoOutput m_videoOutput;
};

#endif // QMLVLCMMPLAYER_H
