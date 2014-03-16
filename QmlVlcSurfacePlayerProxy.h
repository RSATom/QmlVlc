#ifndef QMLVLCSURFACEPLAYERPROXY_H
#define QMLVLCSURFACEPLAYERPROXY_H

#include "QmlVlcPlayerProxy.h"
#include "QmlVlcVideoOutput.h"

class QmlVlcSurfacePlayerProxy
    : public QmlVlcPlayerProxy
{
    Q_OBJECT
public:
    explicit QmlVlcSurfacePlayerProxy( vlc::player* player, QObject* parent = 0 );
    virtual void classBegin();

    void registerVideoSurface( QmlVlcVideoSurface* s )
        { m_videoOutput.registerVideoSurface( s ); }
    void unregisterVideoSurface( QmlVlcVideoSurface* s )
        { m_videoOutput.unregisterVideoSurface( s ); }

private:
    QmlVlcVideoOutput m_videoOutput;
};

#endif // QMLVLCSURFACEPLAYERPROXY_H
