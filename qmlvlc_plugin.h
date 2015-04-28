#pragma once 

#include <QtQml>
#include <QQmlExtensionPlugin>

#include <QmlVlc.h>

#define QMLVLC_PLUGIN_ID "rsatom.qml.vlc"

class Q_DECL_EXPORT QmlVlcExtensionPlugin : public QQmlExtensionPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QMLVLC_PLUGIN_ID)

    public:
        virtual void initializeEngine(QQmlEngine *engine, const char *uri) {}
        virtual void registerTypes(const char *uri) 
        {
            Q_ASSERT(QString(QMLVLC_PLUGIN_ID) == uri);
            RegisterQmlVlc();
        }
};
