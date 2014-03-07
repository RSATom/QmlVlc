#ifndef QMLVLCPLAYER_H
#define QMLVLCPLAYER_H

#include "QmlVlcPlayerProxy.h"
#include "QmlVlcVideoOutput.h"

class QmlVlcPlayer
    : public QmlVlcPlayerProxy
{
    Q_OBJECT
public:
    explicit QmlVlcPlayer( QObject* parent = 0 );
    ~QmlVlcPlayer();

    void registerVideoSurface( QmlVlcVideoSurface* s )
        { m_videoOutput.registerVideoSurface( s ); }
    void unregisterVideoSurface( QmlVlcVideoSurface* s )
        { m_videoOutput.unregisterVideoSurface( s ); }

private:
    libvlc_instance_t* m_libvlc;
    vlc::player m_player;
    QmlVlcVideoOutput m_videoOutput;
};

#endif // QMLVLCPLAYER_H
