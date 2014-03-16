#include "QmlVlcSurfacePlayerProxy.h"

QmlVlcSurfacePlayerProxy::QmlVlcSurfacePlayerProxy( vlc::player* player, QObject* parent )
    : QmlVlcPlayerProxy( player, parent ),
      m_videoOutput( player )
{
}

void QmlVlcSurfacePlayerProxy::classBegin()
{
    QmlVlcPlayerProxy::classBegin();

    m_videoOutput.init();
}
