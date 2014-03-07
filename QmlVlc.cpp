#include "QmlVlc.h"

#include <QtQml/qqml.h>

#include "QmlVlcVideoSurface.h"
#include "QmlVlcPlayer.h"

#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
#include "QmlVlcMmPlayer.h"
#endif

const char* qmlVlcUri = "QmlVlc";
const int QmlVlcVersionMajor = 0;
const int QmlVlcVersionMinor = 1;

void RegisterQmlVlcVideoSurface()
{
    qmlRegisterType<QmlVlcVideoSurface>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcVideoSurface" );
}

void RegisterQmlVlcPlayer( bool creatable = true )
{
    if( creatable ) {
        qmlRegisterType<QmlVlcPlayer>(
            qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
            "VlcPlayer" );
    } else {
        qmlRegisterUncreatableType<QmlVlcPlayer>(
            qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
            "QmlVlcPlayer",
            QStringLiteral("QmlVlcPlayer cannot be instantiated directly") );
    }
}

#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
void RegisterQmlVlcMmPlayer()
{
    qmlRegisterType<QmlVlcMmPlayer>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcMmPlayer" );
}
#endif

void RegisterQmlVlc()
{
    RegisterQmlVlcVideoSurface();
    RegisterQmlVlcPlayer();

#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
    RegisterQmlVlcMmPlayer();
#endif
}
