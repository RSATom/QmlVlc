#pragma once

#include <QtQml>
#include <QQmlExtensionPlugin>

#include "QmlVlc.h"

#define QMLVLC_PLUGIN_ID "RSATom.QmlVlc"

class Q_DECL_EXPORT QmlVlcPlugin
    : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID QMLVLC_PLUGIN_ID )

public:
    virtual void initializeEngine( QQmlEngine* engine, const char* uri );
    virtual void registerTypes( const char* uri ) override;
};
