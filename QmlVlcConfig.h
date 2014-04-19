#ifndef QMLVLCCONFIG_H
#define QMLVLCCONFIG_H

#include <QObject>

#include <vlc/vlc.h>

class QmlVlcConfig
    : public QObject
{
    Q_OBJECT
public:
    static void setNetworkCacheTime( int time )
        { _networkCacheTime = time; }
    static void enableAdjustFilter( bool enable )
        { _adjustFilter = enable; }
    static void enableMarqueeFilter( bool enable )
        { _marqueeFilter = enable; }
    static void enableLogoFilter( bool enable )
        { _logoFilter = enable; }
    static void enableDebug( bool enable )
        { _debug = enable; }

    static libvlc_instance_t* createLibvlcInstance();

private:
    static int _networkCacheTime;
    static bool _adjustFilter;
    static bool _marqueeFilter;
    static bool _logoFilter;
    static bool _debug;
};

#endif // QMLVLCCONFIG_H
