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

void RegisterQmlVlcPlayer()
{
    qmlRegisterUncreatableType<QmlVlcSurfacePlayerProxy>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "QmlVlcSurfacePlayerProxy",
        QStringLiteral("QmlVlcSurfacePlayerProxy cannot be instantiated directly") );
    qmlRegisterType<QmlVlcPlayer>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcPlayer" );
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
