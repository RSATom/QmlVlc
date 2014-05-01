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
    static void enableNoVideoTitleShow( bool enable )
        { _noVideoTitleShow = enable; }
    static void enableHardwareAcceleration( bool enable )
        { _hardwareAcceleration = enable; }

    static libvlc_instance_t* createLibvlcInstance();

private:
    static int _networkCacheTime;
    static bool _adjustFilter;
    static bool _marqueeFilter;
    static bool _logoFilter;
    static bool _debug;
    static bool _noVideoTitleShow;
    static bool _hardwareAcceleration;
};

#endif // QMLVLCCONFIG_H
