#include "QmlVlc.h"

#include <QtQml/qqml.h>

#include "QmlVlcVideoSurface.h"
#include "QmlVlcPlayer.h"

const char* qmlVlcUri = "QmlVlc";
const int QmlVlcVersionMajor = 0;
const int QmlVlcVersionMinor = 1;

void RegisterQmlVlcVideoSurface()
{
    qmlRegisterUncreatableType<QmlVlcVideoOutput>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcVideoOutput",
        QStringLiteral("VlcVideoOutput cannot be instantiated directly") );
    qmlRegisterType<QmlVlcVideoSurface>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcVideoSurface" );
}

void RegisterQmlVlcPlayer()
{
    qmlRegisterType<QmlVlcPlayer>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcPlayer" );
}

void RegisterQmlVlc()
{
    RegisterQmlVlcVideoSurface();
    RegisterQmlVlcPlayer();
}
