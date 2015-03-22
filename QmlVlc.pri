HEADERS += \
    $$PWD/QmlVlc.h \
    $$PWD/QmlVlcConfig.h \
    $$PWD/QmlVlcAudio.h \
    $$PWD/QmlVlcDeinterlace.h \
    $$PWD/QmlVlcInput.h \
    $$PWD/QmlVlcLogo.h \
    $$PWD/QmlVlcMarquee.h \
    $$PWD/QmlVlcMedia.h \
    $$PWD/QmlVlcPlayerProxy.h \
    $$PWD/QmlVlcSurfacePlayerProxy.h \
    $$PWD/QmlVlcPlayer.h \
    $$PWD/QmlVlcPlaylist.h \
    $$PWD/QmlVlcPositions.h \
    $$PWD/QmlVlcSubtitle.h \
    $$PWD/QmlVlcVideo.h \
    $$PWD/QmlVlcVideoFrame.h \
    $$PWD/QmlVlcVideoOutput.h \
    $$PWD/QmlVlcVideoSurface.h \
    $$PWD/SGVlcVideoNode.h

SOURCES += \
    $$PWD/QmlVlc.cpp \
    $$PWD/QmlVlcConfig.cpp \
    $$PWD/QmlVlcAudio.cpp \
    $$PWD/QmlVlcDeinterlace.cpp \
    $$PWD/QmlVlcInput.cpp \
    $$PWD/QmlVlcLogo.cpp \
    $$PWD/QmlVlcMarquee.cpp \
    $$PWD/QmlVlcMedia.cpp \
    $$PWD/QmlVlcPlayerProxy.cpp \
    $$PWD/QmlVlcSurfacePlayerProxy.cpp \
    $$PWD/QmlVlcPlayer.cpp \
    $$PWD/QmlVlcPlaylist.cpp \
    $$PWD/QmlVlcPositions.cpp \
    $$PWD/QmlVlcSubtitle.cpp \
    $$PWD/QmlVlcVideo.cpp \
    $$PWD/QmlVlcVideoFrame.cpp \
    $$PWD/QmlVlcVideoOutput.cpp \
    $$PWD/QmlVlcVideoSurface.cpp \
    $$PWD/SGVlcVideoNode.cpp

contains( QT, multimedia ) {
    DEFINES += QMLVLC_QTMULTIMEDIA_ENABLE

    HEADERS += \
        $$PWD/QmlVlcMmVideoOutput.h \
        $$PWD/QmlVlcMmPlayer.h

    SOURCES += \
        $$PWD/QmlVlcMmVideoOutput.cpp \
        $$PWD/QmlVlcMmPlayer.cpp
}

INCLUDEPATH += $$PWD

include(libvlc_wrapper/libvlc_wrapper.pri)
