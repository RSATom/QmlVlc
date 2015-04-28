include(QmlVlc.pri)

CONFIG += c++11
LIBS += -lvlc
TARGET = qmlvlcplugin

PLUGIN_IMPORT_PATH = rsatom/qml/vlc

TEMPLATE = lib
CONFIG += qt plugin
QT += qml quick

target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH
INSTALLS += target

qmldir.files += $$_PRO_FILE_PWD_/qmldir
qmldir.path += $$target.path
INSTALLS += qmldir

HEADERS += qmlvlc_plugin.h


