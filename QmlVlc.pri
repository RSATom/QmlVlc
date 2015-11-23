include( libvlc_wrapper/libvlc_wrapper.pri )

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
    $$PWD/QmlVlcPlayer.h \
    $$PWD/QmlVlcPlaylist.h \
    $$PWD/QmlVlcPositions.h \
    $$PWD/QmlVlcSubtitle.h \
    $$PWD/QmlVlcVideo.h \
    $$PWD/QmlVlcVideoFrame.h \
    $$PWD/QmlVlcVideoOutput.h \
    $$PWD/QmlVlcVideoSource.h \
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
    $$PWD/QmlVlcPlayer.cpp \
    $$PWD/QmlVlcPlaylist.cpp \
    $$PWD/QmlVlcPositions.cpp \
    $$PWD/QmlVlcSubtitle.cpp \
    $$PWD/QmlVlcVideo.cpp \
    $$PWD/QmlVlcVideoFrame.cpp \
    $$PWD/QmlVlcVideoOutput.cpp \
    $$PWD/QmlVlcVideoSource.cpp \
    $$PWD/QmlVlcVideoSurface.cpp \
    $$PWD/SGVlcVideoNode.cpp

!android {
    HEADERS += \
        $$PWD/QmlVlcMediaListPlayerProxy.h \
        $$PWD/QmlVlcMediaListPlayer.h

    SOURCES += \
       $$PWD/QmlVlcMediaListPlayerProxy.cpp \
       $$PWD/QmlVlcMediaListPlayer.cpp
}

contains( QT, multimedia ) {
    DEFINES += QMLVLC_QTMULTIMEDIA_ENABLE
}

INCLUDEPATH += $$PWD
