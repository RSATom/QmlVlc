#include "QmlVlcPlugin.h"

void QmlVlcPlugin::initializeEngine( QQmlEngine* /*engine*/, const char* /*uri*/ )
{
}

void QmlVlcPlugin::registerTypes( const char* uri )
{
    Q_UNUSED( uri );

    Q_ASSERT( QStringLiteral( QMLVLC_PLUGIN_ID ) == uri );

    RegisterQmlVlc();
}
