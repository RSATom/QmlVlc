#ifndef QMLVLCVIDEOFRAME_H
#define QMLVLCVIDEOFRAME_H

#include <QByteArray>
#include <QMutex>

struct QmlVlcVideoFrame
{
    QByteArray frameBuf;

    QMutex* guard() const
        { return &m_guard; }

private:
    mutable QMutex m_guard;
};

struct QmlVlcI420Frame : public QmlVlcVideoFrame
{
    QmlVlcI420Frame();
    void clear();
    bool isValid();

    quint16 width;
    quint16 height;

    void* yPlane;
    quint32 yPlaneSize;

    void* uPlane;
    quint32 uPlaneSize;

    void* vPlane;
    quint32 vPlaneSize;
};

#endif //QMLVLCVIDEOFRAME_H
