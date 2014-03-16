#ifndef QMLVLCPLAYER_H
#define QMLVLCPLAYER_H

#include "QmlVlcSurfacePlayerProxy.h"

class QmlVlcPlayer
    : public QmlVlcSurfacePlayerProxy
{
    Q_OBJECT
public:
    explicit QmlVlcPlayer( QObject* parent = 0 );
    ~QmlVlcPlayer();

private:
    libvlc_instance_t* m_libvlc;
    vlc::player m_player;
};

#endif // QMLVLCPLAYER_H
