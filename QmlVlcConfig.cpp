#include "QmlVlcConfig.h"

#include <QVector>

int QmlVlcConfig::_networkCacheTime = -1;
bool QmlVlcConfig::_adjustFilter = false;
bool QmlVlcConfig::_marqueeFilter = false;
bool QmlVlcConfig::_logoFilter = false;
bool QmlVlcConfig::_debug = false;

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

    return libvlc_new( opts.size(), opts.data() );
}
