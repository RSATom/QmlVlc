#ifndef QMLVLCVIDEOSURFACE_H
#define QMLVLCVIDEOSURFACE_H

#include <QQuickItem>
#include <QPointer>

#include "QmlVlcPlayer.h"

struct QmlVlcI420Frame;//#include "QmlVlcVideoFrame.h"

class QmlVlcVideoSurface
    : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY( QmlVlcPlayer* source READ source WRITE setSource NOTIFY sourceChanged )

public:
    QmlVlcVideoSurface();
    ~QmlVlcVideoSurface();

    QmlVlcPlayer* source() const
        { return m_source; }
    void setSource( QmlVlcPlayer* source );

    virtual QSGNode* updatePaintNode( QSGNode*, UpdatePaintNodeData* );

public Q_SLOTS:
    void presentFrame( const QSharedPointer<const QmlVlcI420Frame>& frame );

Q_SIGNALS:
    void sourceChanged();

private:
    QPointer<QmlVlcPlayer> m_source;

    QSharedPointer<const QmlVlcI420Frame> m_frame;
};

#endif //QMLVLCVIDEOSURFACE_H
