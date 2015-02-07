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

#include "QmlVlcConfig.h"

#include <QVector>
#include <QStringList>

int QmlVlcConfig::_networkCacheTime = -1;
bool QmlVlcConfig::_adjustFilter = false;
bool QmlVlcConfig::_marqueeFilter = false;
bool QmlVlcConfig::_logoFilter = false;
bool QmlVlcConfig::_debug = false;
bool QmlVlcConfig::_noVideoTitleShow = true;
bool QmlVlcConfig::_hardwareAcceleration = false;
bool QmlVlcConfig::_trustedEnvironment = false;

libvlc_instance_t* QmlVlcConfig::createLibvlcInstance()
{
    QVector<const char*> opts;

    QByteArray networkCachingBuf;
    if( _networkCacheTime >= 0 ) {
        opts.push_back( "--network-caching" );
        networkCachingBuf = QString::number( _networkCacheTime ).toLatin1();
        opts.push_back( networkCachingBuf.constData() );
    }

    if( _adjustFilter )
        opts.push_back( "--video-filter=adjust" );

    QString subFilters;
    if( _marqueeFilter )
        subFilters = QStringLiteral( "marq" );

    if( _logoFilter ) {
        if( !subFilters.isEmpty() )
            subFilters += ':';
        subFilters += QStringLiteral( "logo" );
    }

    QByteArray subFiltersBuf;
    if( !subFilters.isEmpty() ) {
        subFilters = QStringLiteral( "--sub-filter=" ) + subFilters;
        subFiltersBuf = subFilters.toLatin1();
        opts.push_back( subFiltersBuf.constData() );
    }

    if( _debug )
        opts.push_back( "-vvv" );

    if( _noVideoTitleShow )
        opts.push_back( "--no-video-title-show" );

    return libvlc_new( opts.size(), opts.data() );
}

struct TrustedOption
{
    const char* name;
    const char* value; //if 0 = all values are acceptable
};

static const TrustedOption trustedOptions[] = {
    { ":rtsp-http-port", 0 },
    { ":avformat-format", "mxg" },
    { ":demux", "h264" },
    { ":h264-fps", 0 },
};

bool QmlVlcConfig::isOptionTrusted( const QString& opt )
{
    if( trustedEnvironment() )
        return true;

    QStringList name2val = opt.split( '=' );
    if( 2 != name2val.size() )
        return false;

    name2val[0] = name2val[0].trimmed();
    name2val[1] = name2val[1].trimmed();

    const unsigned tsz =
        sizeof( trustedOptions ) / sizeof( trustedOptions[0] );

    for( unsigned i = 0; i < tsz; ++i ) {
        const TrustedOption& to = trustedOptions[i];
        if( name2val[0] == to.name ) {
            if( 0 == to.value || name2val[1] == to.value )
                return true;
        }
    }

    return true;
}
