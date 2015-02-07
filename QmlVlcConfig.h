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

    static void setTrustedEnvironment( bool trusted )
        { _trustedEnvironment = trusted; }
    static bool trustedEnvironment()
        { return _trustedEnvironment; }

    static bool isOptionTrusted( const QString& );

private:
    static int _networkCacheTime;
    static bool _adjustFilter;
    static bool _marqueeFilter;
    static bool _logoFilter;
    static bool _debug;
    static bool _noVideoTitleShow;
    static bool _hardwareAcceleration;
    static bool _trustedEnvironment;
};

#endif // QMLVLCCONFIG_H
