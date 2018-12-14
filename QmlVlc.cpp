/*******************************************************************************
* Copyright © 2014, Sergey Radionov <rsatom_gmail.com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.

* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include "QmlVlc.h"

#include <QtQml/qqml.h>

#include "QmlVlcVideoSurface.h"
#include "QmlVlcPlayer.h"

#ifndef Q_OS_ANDROID
    #include "QmlVlcMediaListPlayer.h"
#endif

const char* qmlVlcUri = "RSATom.QmlVlc";
const int QmlVlcVersionMajor = 0;
const int QmlVlcVersionMinor = 1;

void RegisterQmlVlcPlayer(const char* uri /*= nullptr*/)
{
    // @uri QmlVlc
    if (uri == nullptr)
    {
        uri = qmlVlcUri;
    }

    qmlRegisterUncreatableType<QmlVlcVideoSource>(
        uri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "QmlVlcVideoSource",
        QStringLiteral("QmlVlcVideoSource cannot be instantiated directly") );

    qmlRegisterUncreatableType<QmlVlcInput>(
        uri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcInput",
        QStringLiteral("VlcInput cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcVideo>(
        uri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcVideo",
        QStringLiteral("VlcVideo cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcAudio>(
        uri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcAudio",
        QStringLiteral("VlcAudio cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcSubtitle>(
        uri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcSubtitle",
        QStringLiteral("VlcSubtitle cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcPlaylist>(
        uri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcPlaylist",
        QStringLiteral("VlcPlaylist cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcMarquee>(
        uri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcMarquee",
        QStringLiteral("VlcMarquee cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcLogo>(
        uri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcLogo",
        QStringLiteral("VlcLogo cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcDeinterlace>(
        uri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcDeinterlace",
        QStringLiteral("VlcDeinterlace cannot be instantiated directly") );
    qmlRegisterUncreatableType<QmlVlcMedia>(
        uri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcMediaDesc",
        QStringLiteral("VlcMediaDesc cannot be instantiated directly") );

    qmlRegisterType<QmlVlcVideoSurface>(
        uri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcVideoSurface" );
    qmlRegisterType<QmlVlcPlayer>(
        uri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcPlayer" );

#ifndef Q_OS_ANDROID
    qmlRegisterType<QmlVlcMediaListPlayer>(
        uri, QmlVlcVersionMajor, QmlVlcVersionMinor,
        "VlcMediaListPlayer" );
#endif
}

void RegisterQmlVlc()
{
    RegisterQmlVlcPlayer();
}
