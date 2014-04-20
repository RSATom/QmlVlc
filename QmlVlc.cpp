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

void RegisterQmlVlcPlayer()
{
    // @uri QmlVlc
    qmlRegisterUncreatableType<QmlVlcAudio>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcAudio",
        QStringLiteral("VlcAudio cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcInput>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcInput",
        QStringLiteral("VlcInput cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcPlaylistItems>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcPlaylistItems",
        QStringLiteral("VlcPlaylistItems cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcPlaylist>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcPlaylist",
        QStringLiteral("VlcPlaylist cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcSubtitle>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcSubtitle",
        QStringLiteral("VlcSubtitle cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcMarquee>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcMarquee",
        QStringLiteral("VlcMarquee cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcLogo>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcLogo",
        QStringLiteral("VlcLogo cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcDeinterlace>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcDeinterlace",
        QStringLiteral("VlcDeinterlace cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcVideo>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcVideo",
        QStringLiteral("VlcVideo cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcMediaDesc>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcMediaDesc",
        QStringLiteral("VlcMediaDesc cannot be instantiated directly") );

    qmlRegisterUncreatableType<QmlVlcSurfacePlayerProxy>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcSurfacePlayerProxy",
        QStringLiteral("VlcSurfacePlayerProxy cannot be instantiated directly") );

    qmlRegisterType<QmlVlcVideoSurface>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcVideoSurface" );
    qmlRegisterType<QmlVlcPlayer>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcPlayer" );
}

#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
void RegisterQmlVlcMmPlayer()
{
    // @uri QmlVlc
    qmlRegisterType<QmlVlcMmPlayer>(
        qmlVlcUri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcMmPlayer" );
}
#endif

void RegisterQmlVlc()
{
    RegisterQmlVlcPlayer();

#ifdef QMLVLC_QTMULTIMEDIA_ENABLE
    RegisterQmlVlcMmPlayer();
#endif
}
